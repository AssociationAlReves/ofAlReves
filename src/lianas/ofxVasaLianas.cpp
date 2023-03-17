#include "ofxVasaLianas.h"
#include "ofApp.h"


using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void ofxVasaLianas::setup() {

	if (!guiInitialized) {
		initGui();
		initKinect();
	}
	initLianas();
	
}

//--------------------------------------------------------------
void ofxVasaLianas::initGui() {

	gui.setup("panel", Globals::hostName + LIANAS_SETTINGS_FILE); // most of the time you don't need a name but don't forget to call setup

	//---------------------------------------------------
	/// Lianas
	zebraParams.setName("Main");
	zebraParams.add(randomNodes.set("randomNodes", true));
	zebraParams.add(lockX.set("lockX", false));
	zebraParams.add(lockY.set("lockY", true));
	zebraParams.add(lockZ.set("lockZ", true));
	zebraParams.add(lockLastNode.set("lockLastNode", true));
	zebraParams.add(numLianas.set("numLianas", 1, 1, 100));
	zebraParams.add(gravity.set("gravity", 0, -1, 1));
	zebraParams.add(springLineWidth.set("lineWidth", 2, 0, 20));
	
	gui.add(zebraParams);

	nodeParams.setName("Nodes");
	nodeParams.add(nodeRadius.set("nodeRadius", 100, 1, 200));
	nodeParams.add(nodeStrength.set("nodeStrength", -5, -100, 100));
	nodeParams.add(nodeDiameter.set("nodeDiameter", 16, 1, 200));
	nodeParams.add(nodeDamping.set("nodeDamping", 0.1f, -1, 1));
	nodeParams.add(nodeRamp.set("nodeRamp", 1.f, -50, 50));
	nodeParams.add(nodeVelocity.set("nodeVelocity", 0.99f, -1, 50));
	gui.add(nodeParams);

	springParams.setName("Springs");
	springParams.add(numNodes.set("numNodes", 20, 2, 100));
	springParams.add(springLength.set("springLength", 20, 0, 500));
	springParams.add(springStiffness.set("springStiffness", 3, 0, 50));
	springParams.add(stringDamping.set("stringDamping", 0.9, 0.9, 1));
	gui.add(springParams);

	repulsionParams.setName("Repulsion");
	repulsionParams.add(repulsionRadius.set("repulsionRadius", 50, 0, 500));
	repulsionParams.add(repulsionStrength.set("rep.Strength", 5, -1500, 1500));
	gui.add(repulsionParams);

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
	debugGroup.add(screenBounds.set("bounds", ofVec2f(ofGetWidth(),ofGetHeight()),ofVec2f(-ofGetWidth(),-ofGetHeight()),ofVec2f(ofGetWidth(),ofGetHeight())));
	debugGroup.add(screenTopLeftPos.set("topleftPos", ofVec2f(0),ofVec2f(-ofGetWidth(),-ofGetHeight()),ofVec2f(ofGetWidth(),ofGetHeight())));
	
	
	gui.add(debugGroup);
	bShowGui = false;
	guiInitialized = true;

}

//--------------------------------------------------------------
void ofxVasaLianas::initLianas() {
	
	bUseMouseRepulsor = false;
	bUseKinectRepulsor = false;
	
	if (bLianas) {
		for (int i = 0; i < lianas.size(); i++) {
			lianas[i]->cleanUp();
			delete lianas[i];
		}
		lianas.clear();
		
		// Create lianas centered
		float spacing = ofGetWidth() / (numLianas + 1);
		for (int i = 0; i < numLianas; i++) {
			
			// use ofxLiana() to use original random node placement
			// use ofxLiana(x) to have all nodes on same X
			ofxLiana* liana = randomNodes ? new ofxLiana() : new ofxLiana(spacing * (i + 1));
			liana->numNodes = numNodes;
			liana->nodeRadius = nodeRadius;
			liana->nodeStrength = nodeStrength;
			liana->nodeDiameter = nodeDiameter;
			liana->nodeDamping = nodeDamping;
			liana->nodeRamp = nodeRamp;
			liana->nodeVelocity = nodeVelocity;
			
			liana->springLength = springLength;
			liana->springStiffness = springStiffness;
			liana->stringDamping = stringDamping;
			
			liana->gravity = gravity;
			liana->lineWidth = springLineWidth;
			liana->lockLastNode = lockLastNode;
			
			//
			// setup liana with X position
			liana->setup();
			
			lianas.push_back(liana);
			
		}
		
	} else {
		lianaMesh.cleanUp();
		
		
		// Create lianas centered
		lianaMesh.numNodes = numNodes;
		lianaMesh.nodeRadius = nodeRadius;
		lianaMesh.nodeStrength = nodeStrength;
		lianaMesh.nodeDiameter = nodeDiameter;
		lianaMesh.nodeDamping = nodeDamping;
		lianaMesh.nodeRamp = nodeRamp;
		lianaMesh.nodeVelocity = nodeVelocity;
		
		lianaMesh.springLength = springLength;
		lianaMesh.springStiffness = springStiffness;
		lianaMesh.stringDamping = stringDamping;
		
		lianaMesh.gravity = gravity;
		lianaMesh.lineWidth = springLineWidth;
		lianaMesh.lockLastNode = lockLastNode;
		
		lianaMesh.setup();
	}
}

//--------------------------------------------------------------
void ofxVasaLianas::initKinect() {

	if (!kinect.isConnected()) {
	// enable depth->video image calibration
	kinect.setRegistration(true);
	kinect.init();
	kinect.open();		// opens first available kinect
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
void ofxVasaLianas::update() {
	
	if (bLianas) {
		for (int i = 0; i < lianas.size(); i++) {
			
			
			//
			// set updated params
			lianas[i]->numNodes = numNodes;
			lianas[i]->nodeRadius = nodeRadius;
			lianas[i]->nodeStrength = nodeStrength;
			lianas[i]->nodeDiameter = nodeDiameter;
			lianas[i]->nodeDamping = nodeDamping;
			lianas[i]->nodeRamp = nodeRamp;
			lianas[i]->nodeVelocity = nodeVelocity;
			
			lianas[i]->springLength = springLength;
			lianas[i]->springStiffness = springStiffness;
			lianas[i]->stringDamping = stringDamping;
			
			lianas[i]->gravity = gravity;
			lianas[i]->lineWidth = springLineWidth;
			lianas[i]->repulsionStrength = repulsionStrength;
			lianas[i]->repulsionRadius = repulsionRadius;//
			// update
			lianas[i]->update(lockX, lockY, lockZ);
			if (bUseKinectRepulsor) {
				lianas[i]->keyPressed(' ');
			}
            if (bUseMouseRepulsor && ofGetKeyPressed(' ')) {
                lianas[i]->keyPressed(' ');
                lianas[i]->addRepulsor(ofGetMouseX(),ofGetMouseY() );
            }
			
			lianas[i]->clearRepulsors();
		}
	} else {
		//
		// set updated params
		lianaMesh.numNodes = numNodes;
		lianaMesh.nodeRadius = nodeRadius;
		lianaMesh.nodeStrength = nodeStrength;
		lianaMesh.nodeDiameter = nodeDiameter;
		lianaMesh.nodeDamping = nodeDamping;
		lianaMesh.nodeRamp = nodeRamp;
		lianaMesh.nodeVelocity = nodeVelocity;
		
		lianaMesh.springLength = springLength;
		lianaMesh.springStiffness = springStiffness;
		lianaMesh.stringDamping = stringDamping;
		
		lianaMesh.gravity = gravity;
		lianaMesh.lineWidth = springLineWidth;
		lianaMesh.repulsionStrength = repulsionStrength;
		lianaMesh.repulsionRadius = repulsionRadius;
		
		//
		// update
		lianaMesh.update(lockX, lockY, lockZ);
		
		if (bUseKinectRepulsor) {
			lianaMesh.keyPressed(' ');
		}
		
		lianaMesh.clearRepulsors();
	}
	
	
	updateKinect();
}

//--------------------------------------------------------------
void ofxVasaLianas::updateKinect() {
	
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

//--------------------------------------------------------------
void ofxVasaLianas::draw() {

	ofClear(0);
	//ofApp *app = (ofApp *)ofxGetAppPtr();
//	if (easyCamMouse) {
//		app->cam.enableMouseInput();
//	} else {
//		app->cam.disableMouseInput();
//	}
	
	//ofScale(1, -1, 1);
	//ofTranslate(0, ofGetScreenHeight());
	//cam.begin();
	//ofClear(0);
	ofSetColor(255);
	
	
	
	//cam.begin();
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	
	ofScale(abs(screenBounds->x)/ofGetWidth(), screenBounds->y/ofGetHeight());
	ofTranslate(screenTopLeftPos->x, screenTopLeftPos->y);
	
	ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
	if (bLianas)
	{
		for (int i = 0; i < lianas.size(); i++) {
			lianas[i]->draw();
		}
	} else {
		lianaMesh.draw();
	}
	ofPopMatrix();
	
	ofPushMatrix();
	
	ofScale(kwScaleX, kwScaleY);
	ofTranslate(kwX, kwY);
	
    if (bUseKinectRepulsor) {
        drawKinect();
    }
	
	if (kinectWarp) {
		ofSetColor(255);
		ofSetLineWidth(5);
		ofDrawLine(0,0,kinect.width,0);
		ofDrawLine(kinect.width, 0, kinect.width, kinect.height);
		ofDrawLine(kinect.width, kinect.height,0, kinect.height);
		ofDrawLine(0, kinect.height, 0, 0);
		ofSetLineWidth(1);
	}
	
	ofPopMatrix();
	
	
	
//	app->cam.end();
	
	
	//cam.end();
	
	if (bShowGui)
	{
        stringstream ss;
        ss << "FPS : " + ofToString(ofGetFrameRate());
        ofDrawBitmapStringHighlight(ss.str(), 10, 10);
		gui.draw();
	}
	//app->cam.begin();
}

//--------------------------------------------------------------
void ofxVasaLianas::drawKinect() {
	
	
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
				if (bLianas) {
					for (int i = 0; i < lianas.size(); i++) {
						lianas[i]->addRepulsor(cPoint.x*kwScaleX+kwX, cPoint.y*kwScaleY+kwY);
					}
				} else {
					lianaMesh.addRepulsor(cPoint.x*kwScaleX+kwX, cPoint.y*kwScaleY+kwY);
				}
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
void ofxVasaLianas::updateExit() {
	closeKinect();
	finishedExiting();
}
//--------------------------------------------------------------
void ofxVasaLianas::exit() {
	closeKinect();
}
//--------------------------------------------------------------
void ofxVasaLianas::closeKinect() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}
//--------------------------------------------------------------
void ofxVasaLianas::setupKinectWarp(bool shift, bool alt, int x, int y) {
}

//--------------------------------------------------------------
void ofxVasaLianas::keyPressed(int key) {
	
	//ofApp *app = (ofApp *)ofxGetAppPtr();
	
	switch (key) {
		case 'r': initLianas(); break;
		case 'h': bShowGui = !bShowGui;
			if (bShowGui) {
				//app->cam.disableMouseInput();
			} else {
                //app->cam.enableMouseInput();
			}
			break;
		case 'l':
            { if (bLianas)
                gui.loadFromFile(Globals::hostName + "lianas.xml");
            else
                gui.loadFromFile(Globals::hostName + "lianamesh.xml");
            }
            break;
		case 'j': bUseMouseRepulsor = false; break;
        case 'J' : {
            lianaMesh.clearRepulsors();
            bUseMouseRepulsor = true;
        }
            break;
		case 'k': bUseKinectRepulsor = false; break;
		case 'K' : bUseKinectRepulsor = true; break;
		case 'y' : {bLianas = !bLianas; initLianas();} break;
		case 's': { if (bLianas)
			gui.saveToFile(Globals::hostName + "lianas.xml");
		else
			gui.saveToFile(Globals::hostName + "lianamesh.xml");
		}
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
	
	if (bLianas) {
		for (int i = 0; i < lianas.size(); i++) {
			lianas[i]->keyPressed(key);
		}
	} else {
	lianaMesh.keyPressed(key);
	}
}

//--------------------------------------------------------------
void ofxVasaLianas::keyReleased(int key) {
	if (bLianas) {
		for (int i = 0; i < lianas.size(); i++) {
			lianas[i]->keyReleased(key);
		}
	} else {
		lianaMesh.keyReleased(key);
	}
}

//--------------------------------------------------------------
void ofxVasaLianas::mouseMoved(int x, int y) {

	
	if (bUseMouseRepulsor) {
		if (bLianas) {
			for (int i = 0; i < lianas.size(); i++) {
				lianas[i]->addRepulsor(x, y );
			}
		} else {
			lianaMesh.addRepulsor(x, y );
		}
	}
}

//--------------------------------------------------------------
void ofxVasaLianas::mousePressed(int x, int y, int button) {
	if (bLianas) {
		for (int i = 0; i < lianas.size(); i++) {
			lianas[i]->mousePressed(x, y, button );
		}
	} else {
		lianaMesh.mousePressed(x, y, button );
	}
}

//--------------------------------------------------------------
void ofxVasaLianas::mouseReleased(int x, int y, int button) {
	if (bLianas) {
		for (int i = 0; i < lianas.size(); i++) {
			lianas[i]->mouseReleased(x, y, button );
		}
	} else {
		lianaMesh.mouseReleased(x, y, button );
	}
}
