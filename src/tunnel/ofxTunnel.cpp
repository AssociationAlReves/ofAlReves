//
//  ofxTunnel.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 03/04/2018.
//

#include "ofxTunnel.h"
#include "ofApp.h"

using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void ofxTunnel::setup() {
    
    if (!guiInitialized) {
        initGui();
        initKinect();
    }
    screenWidth = ofGetWidth();
    screenHeight = ofGetHeight();
}

//--------------------------------------------------------------
void ofxTunnel::initGui() {
    
    gui.setup("panel", Globals::hostName + TUNNEL_SETTINGS_FILE); // most of the time you don't need a name but don't forget to call setup
    
    //---------------------------------------------------
    /// Tunnel-related
    tunnelGroup.setName("Tunnel");
    tunnelGroup.add(tunnelSteps.set("steps", 20, 4, 100));
    tunnelGroup.add(tunnelSpeed.set("velocity", 100, 0, 500));
    tunnelGroup.add(tunnelHueSpeed.set("HSV (H)", 0.01, 0, 0.5));
    tunnelGroup.add(tunnelSaturationSpeed.set("HSV (S)", 0.0034, 0, 0.005));
    tunnelGroup.add(tunnelBrightnessSpeed.set("HSV (V)", 0.001, 0, 0.001));
    tunnelGroup.add(tunnelRotation.set("Rotation", 0.1, 0, 1));
    gui.add(tunnelGroup);

    //---------------------------------------------------
    /// Kinect-related
    cvGroup.setName("OpenCV");
    cvGroup.add(nearThreshold.set("nearThreshold", 255, 0, 255));
    cvGroup.add(farThreshold.set("farThreshold", 213, 0, 255));
    cvGroup.add(thresholdParam.set("threshold", 13, 0, 255));
    cvGroup.add(contourMinArea.set("contourMinArea", 1, 0, 640));
    cvGroup.add(contourMaxArea.set("contourMaxArea", 1000000, 0, 1000000));
    cvGroup.add(blurSize.set("blurSize", 10, 0, 50));
    cvGroup.add(maximumDistance.set("maximumDistance", 32, 0, 300));
    cvGroup.add(persistence.set("persistence", 60, 0, 100));
    gui.add(cvGroup);
    
    debugGroup.setName("debug");
    debugGroup.add(bShowLabels.set("ShowLabels", true));
    debugGroup.add(bShowDepth.set("bShowDepth", false));
    debugGroup.add(bShowFilteredDepth.set("bShowFilteredDepth", false));
    debugGroup.add(angle.set("kinect angle", 0, -30, 30));
    debugGroup.add(kwScaleX.set("kinect warp sX", 1,0,3));
    debugGroup.add(kwScaleY.set("kinect warp sY", 1,0,3));
    debugGroup.add(kwX.set("kinect X", 0,-100,1000));
    debugGroup.add(kwY.set("kinect Y", 0,-100,800));
    debugGroup.add(kinectWarp.set("kinect warp", false));
    debugGroup.add(easyCamMouse.set("easyCamMouse", true));
    debugGroup.add(screenBounds.set("bounds", ofVec2f(screenWidth,screenHeight),ofVec2f(-screenWidth,-screenHeight),ofVec2f(screenWidth,screenHeight)));
    debugGroup.add(screenTopLeftPos.set("topleftPos", ofVec2f(0),ofVec2f(-screenWidth,-screenHeight),ofVec2f(screenWidth,screenHeight)));
    
    gui.add(debugGroup);
    bShowGui = false;
    guiInitialized = true;
    
}



//--------------------------------------------------------------
void ofxTunnel::initKinect() {
    
    if (!kinect.isConnected()) {
        // enable depth->video image calibration
        kinect.setRegistration(true);
        kinect.init();
        kinect.open();        // opens first available kinect
        // print the intrinsic IR sensor values
    }
    if (kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    bKinectFrameReady = false;
    
    
    //-----------------------------------
    // contour finder
    contourFinder.setMinAreaRadius(1);
    contourFinder.setMaxAreaRadius(800);
    contourFinder.setThreshold(15);
    // wait for half a frame before forgetting something
    contourFinder.getTracker().setPersistence(15);
    // an object can move up to 32 pixels per frame
    contourFinder.getTracker().setMaximumDistance(50); //32
    
    grayImage.allocate(kinect.width, kinect.height, ofImageType::OF_IMAGE_GRAYSCALE);
    grayImage.clear();
    
}

//--------------------------------------------------------------
void ofxTunnel::update() {
    
    updateKinect();
}

//--------------------------------------------------------------
void ofxTunnel::updateKinect() {
    
    if (kinect.isConnected()) {
        contourFinder.setMinAreaRadius(contourMinArea);
        contourFinder.setMaxAreaRadius(contourMaxArea);
        contourFinder.setThreshold(thresholdParam);
        contourFinder.getTracker().setPersistence(persistence);
        contourFinder.getTracker().setMaximumDistance(maximumDistance);
        
        kinect.update();
        
        // there is a new frame and we are connected
        if (kinect.isFrameNew()) {
            bKinectFrameReady = true;
            // load grayscale depth image and color image from the kinect source
            //grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
            grayImage.setFromPixels(kinect.getDepthPixels());
            //grayImage.update();
            
            copyGray(grayImage, grayImageNear);
            copyGray(grayImage, grayImageFar);
            imitate(grayImageFiltered, grayImage);
            
            threshold(grayImageNear, (float)nearThreshold, true);
            threshold(grayImageFar, (float)farThreshold, false);
            bitwise_and(grayImageNear, grayImageFar, grayImageFiltered);
            grayImageFiltered.update();
            
            blur(grayImageFiltered, blurSize);
            grayImageFiltered.update();
            contourFinder.findContours(grayImageFiltered);
        }
    }
}
float time1 = 0;
float time2 = 0;
//--------------------------------------------------------------
void ofxTunnel::draw() {
    
    float refTime = ofGetElapsedTimef();
    ofClear(128, 0, 0);
    
    ofSetColor(255);
    
    
    
    ofPushMatrix();
    
    // draw
    // we use one hue (value from 0..255) for the whole grid. it changes over time. we use fmodf to
    // keep the hue value between 0 and 255, it works just like integer modulo (the % operator) but
    // for floats.
    
    
    
    ofPoint center = ofPoint(screenWidth/2, screenHeight/2);
    int step = tunnelSteps;
    int iter = 0;
    float sinDelta;
    
    // step through horizontally
    for ( int i=0; i<screenWidth; i+=step )
    {
        iter++;
        ofColor c;
        // the range of each of the arguments here is 0..255 so we map i and j to that range.


        float rectWidth = screenWidth - i;
        sinDelta = fmodf(ofGetElapsedTimef()*tunnelSpeed, step*2);
        ofPoint newCenter = ofPoint(center.x-rectWidth/2-sinDelta/2,center.y-rectWidth/2-sinDelta/2);

        float hue = sin(ofGetElapsedTimef()*tunnelHueSpeed)*255;
        float saturation = sin(ofGetElapsedTimef()*(tunnelSaturationSpeed * newCenter.x))*255;
        float brightness = 128+cos(ofGetElapsedTimef()*(tunnelBrightnessSpeed * newCenter.x))*128;
        c.setHsb( hue, saturation, brightness);

        // assign the color and draw a rectangle
        if (iter % 2 == 0)
        {
            continue;
            ofSetColor( 255 );
        }
        else
        {
            ofSetColor( c );
        }
        
        // Curve effect
        // The more distant the more offset
        // => the closer is i to 0
        ofPoint mouseCentered;
        if (Globals::oscMouseX >=0) {
            mouseCentered = ofPoint(ofGetMouseX()-screenWidth/2,ofGetMouseY()-screenHeight);
//             mouseCentered = ofPoint(Globals::oscMouseX-screenWidth/2,Globals::oscMouseY-screenHeight);
        } else {
            mouseCentered = ofPoint(ofGetMouseX()-screenWidth/2,ofGetMouseY()-screenHeight);
        }
        ofPoint curveDisplacement = ofPoint(ofMap(rectWidth, 0, screenWidth, mouseCentered.x, 0), ofMap(rectWidth, 0, screenWidth, mouseCentered.y, 0));

        newCenter.x += curveDisplacement.x;
        newCenter.y += curveDisplacement.y;

       
        ofTranslate(screenWidth/2, screenHeight/2);
        ofRotate(sin(ofGetElapsedTimef()*tunnelRotation) * ofMap(rectWidth,0,screenWidth,45.0,0), 0, 0, 1);
        ofTranslate(-screenWidth/2, -screenHeight/2);
        
        drawTunnelPart(center.x, center.y, 0, rectWidth, rectWidth, screenWidth/step, c);

        //ofDrawRectangle(newCenter.x, newCenter.y, rectWidth+sinDelta, rectWidth+sinDelta);
       
    }
    time2 += ofGetElapsedTimef() - refTime;
    refTime = ofGetElapsedTimef();
    ofPopMatrix();
    
    
    if (bShowGui)
    {stringstream ss;
        ss << ofToString(iter) << endl;
        ss << "FPS : " + ofToString(ofGetFrameRate()) << endl
        << "time1: " << ofToString(time1) << endl
        << "time2: " << ofToString(time2) << endl;
        ofDrawBitmapStringHighlight(ss.str(), 10, 10);
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofxTunnel::drawKinect() {
    
    
    if (bKinectFrameReady) {
        
        RectTracker& tracker = contourFinder.getTracker();
        
        // delete dead actors
        for (auto & label : tracker.getDeadLabels())
        {
            //cout << "Dead actor: " << label << endl;
            actors.erase(label);
            actorsHulls.erase(label);
        }
        // delete new actors
        for (auto & label : tracker.getNewLabels())
        {
            //cout << "New actor: " << label << endl;
            actors[label] = list<vector<cv::Point> >();
        }
        
        
        
        // for each blob
        for (int i = 0; i < contourFinder.size(); i++) {
            
            int label = contourFinder.getLabel(i);
            contourFinder.getCentroid(i);
            vector<cv::Point> hullPoints = contourFinder.getConvexHull(i);
            
            cv::Point2f centroid = contourFinder.getCentroid(i);
            //cout<< "c" << centroid.y << endl;
            //cout<< "c1" << centroid.y*kwScaleY-kwY << endl;
            
            //lianas[i]->addRepulsor(centroid.x*kwScaleX+kwX, centroid.y*kwScaleY+kwY);
            for (auto & cPoint : hullPoints)
            {
//                if (bLianas) {
//                    for (int i = 0; i < lianas.size(); i++) {
//                        lianas[i]->addRepulsor(cPoint.x*kwScaleX+kwX, cPoint.y*kwScaleY+kwY);
//                    }
//                } else {
//                    lianaMesh.addRepulsor(cPoint.x*kwScaleX+kwX, cPoint.y*kwScaleY+kwY);
//                }
            }
            
            
            
            
            
            if (bShowLabels) {
                ofPolyline polyline;
                polyline.resize(hullPoints.size());
                for (int hullIndex = 0; hullIndex < (int)hullPoints.size(); hullIndex++) {
                    polyline[hullIndex].x = hullPoints[hullIndex].x;
                    polyline[hullIndex].y = hullPoints[hullIndex].y;
                }
                polyline.close();
                polyline.draw();
                ofPoint center = toOf(contourFinder.getCenter(i));
                ofPushMatrix();
                ofTranslate(center.x, center.y);
                string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
                ofDrawBitmapStringHighlight(msg, 0, 0, ofColor::white, ofColor::red);
                ofVec2f velocity = toOf(contourFinder.getVelocity(i));
                ofScale(5, 5);
                ofDrawLine(0, 0, velocity.x, velocity.y);
                
                ofPopMatrix();
            }
        } // for each blob
        
        
        if (bShowDepth) {
            ofPushMatrix();
            ofScale(1,-1);
            ofTranslate(0, -kinect.height);
            grayImage.draw(0, 0);
            ofPopMatrix();
        } else if (bShowFilteredDepth) {
            ofPushMatrix();
            ofScale(1,-1);
            ofTranslate(0, -kinect.height);
            grayImageFiltered.draw(0,0);
            ofPopMatrix();
        }
    } // END if KINECT
    
}

//--------------------------------------------------------------
void ofxTunnel::drawTunnelPart(float x, float y, float z, float w, float h, float bandWidth, ofColor const & color) {
   
    
    //ofDrawRectangle(x, y, w, h);
    //ofSetPolyMode(OF_POLY_WINDING_NONZERO);
    ofSetLineWidth(bandWidth);
    ofPolyline polyline;
    polyline.resize(4);
   polyline.addVertex(x-w,y-w);
    polyline.addVertex(x-w,y+w);
    polyline.addVertex(x+w,y+w);
    polyline.addVertex(x+w,y-w);
    polyline.close(); 
    polyline.draw();
    
//    ofNextContour(true);
//
//    ofVertex(x-w+bandWidth,y-w+bandWidth,z);
//    ofVertex(x-w+bandWidth,y+w-bandWidth,z);
//    ofVertex(x+w-bandWidth,y+w-bandWidth,z);
//    ofVertex(x+w-bandWidth,y-w+bandWidth,z);
//
//    ofNextContour(true);
    
    //ofEndShape(true);
    ofSetLineWidth(1);
    
}

//--------------------------------------------------------------
void ofxTunnel::updateExit() {
    closeKinect();
    finishedExiting();
}
//--------------------------------------------------------------
void ofxTunnel::exit() {
    closeKinect();
}
//--------------------------------------------------------------
void ofxTunnel::closeKinect() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
}
//--------------------------------------------------------------
void ofxTunnel::setupKinectWarp(bool shift, bool alt, int x, int y) {
}

//--------------------------------------------------------------
void ofxTunnel::keyPressed(int key) {
    
    //ofApp *app = (ofApp *)ofxGetAppPtr();
    
    switch (key) {
        case 'r':  break;
        case 'h': bShowGui = !bShowGui;
//            if (bShowGui) {
//                app->cam.disableMouseInput();
//            } else {
//                app->cam.enableMouseInput();
//            }
            break;
        case 'l':
            gui.loadFromFile(Globals::hostName + Globals::hostName + TUNNEL_SETTINGS_FILE);
            break;
        case 'j': bUseMouseRepulsor = false; break;
        case 'J' : bUseMouseRepulsor = true; break;
        case 'k': bUseKinectRepulsor = false; break;
        case 'K' : bUseKinectRepulsor = true; break;
         case 's': gui.saveToFile(Globals::hostName + TUNNEL_SETTINGS_FILE);
            break;
        case 'C':
            kinect.setCameraTiltAngle(0); // zero the tilt on exit
            kinect.close();
            break;
        case 'O':
            kinect.open();
            kinect.setCameraTiltAngle(angle);
            break;
        case OF_KEY_UP:
            angle++;
            if (angle > 30) angle = 30;
            kinect.setCameraTiltAngle(angle);
            break;
        case OF_KEY_DOWN:
            angle--;
            if (angle < -30) angle = -30;
            kinect.setCameraTiltAngle(angle);
            break;
            //case 'C': cam.enableMouseInput(); break;
            //case 'c': cam.disableMouseInput(); break;
    }
    
}

//--------------------------------------------------------------
void ofxTunnel::windowResized(int w, int h){
    screenWidth = w;
    screenHeight = h;
}

//--------------------------------------------------------------
void ofxTunnel::keyReleased(int key) {
   
}

//--------------------------------------------------------------
void ofxTunnel::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofxTunnel::mousePressed(int x, int y, int button) {
   
}

//--------------------------------------------------------------
void ofxTunnel::mouseReleased(int x, int y, int button) {
    
}
