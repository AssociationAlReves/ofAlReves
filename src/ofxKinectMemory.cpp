#include "ofxKinectMemory.h"
#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

const string GUI_SETTINGS = "kinectmemory_settings.xml";  

//--------------------------------------------------------------
void ofxKinectMemory::setup(){
	ofSetVerticalSync(true);
	ofBackground(0);

	// enable depth->video image calibration
	kinect.setRegistration(true);
	kinect.init();
	kinect.open();		// opens first available kinect
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}


	bShowHelp = false;
	bGotImage = false;

	grayImage.allocate(kinect.width, kinect.width, ofImageType::OF_IMAGE_GRAYSCALE);
	grayImage.clear();

	//-----------------------------------
	// contour finder
	contourFinder.setMinAreaRadius(1);
	contourFinder.setMaxAreaRadius(800);
	contourFinder.setThreshold(15);
	// wait for half a frame before forgetting something
	contourFinder.getTracker().setPersistence(15);
	// an object can move up to 32 pixels per frame
	contourFinder.getTracker().setMaximumDistance(32);

	// -----------------------
	// GUI

	gui.setup("Memory",GUI_SETTINGS);

	cvGroup.setName("OpenCV");
	cvGroup.add(nearThreshold.set("nearThreshold",255,0,255));	
	cvGroup.add(farThreshold.set("farThreshold", 213,0,255));	
	cvGroup.add(thresholdParam.set("threshold", 13,0,255));
	cvGroup.add(contourMinArea.set("contourMinArea",1,0,640));	
	cvGroup.add(contourMaxArea.set("contourMaxArea",800,0,640));
	cvGroup.add(blurSize.set("blurSize",10,0,50));
	gui.add(cvGroup);

	appGroup.setName("App");
	appGroup.add(numFramesDelay.set("numFramesDelay",35,1,50));
	appGroup.add(angle.set("angle",13,-30,30));
	gui.add(appGroup);

	debugGroup.setName("debug");
	debugGroup.add(bShowLabels.set("ShowLabels",true));
	debugGroup.add(bShowImages.set("ShowImages",true));
	gui.add(debugGroup);



}

//--------------------------------------------------------------
void ofxKinectMemory::update(){

	contourFinder.setMinAreaRadius(contourMinArea);
	contourFinder.setMaxAreaRadius(contourMaxArea);
	contourFinder.setThreshold(thresholdParam);

	kinect.update();

	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		bGotImage = true;
		// load grayscale depth image and color image from the kinect source
		//grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		grayImage.setFromPixels(kinect.getDepthPixels(),kinect.width, kinect.height, ofImageType::OF_IMAGE_GRAYSCALE, false);
		//grayImage.update();

		copyGray(grayImage,grayImageNear);		
		copyGray(grayImage,grayImageFar);
		imitate(grayImageFiltered,grayImage);

		threshold(grayImageNear, (float)nearThreshold, true);		
		threshold(grayImageFar, (float)farThreshold, false);
		bitwise_and(grayImageNear, grayImageFar, grayImageFiltered);
		grayImageFiltered.update();


		blur(grayImageFiltered, blurSize);
		grayImageFiltered.update();
		contourFinder.findContours(grayImageFiltered);

		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds

		/*grayThreshNear = grayImage;
		grayThreshFar = grayImage;
		threshold(grayThreshNear, nearThreshold, true);
		threshold(grayThreshFar, farThreshold);
		cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		*/


		// update the cv images
		/*grayImage.flagImageChanged();*/

	}


}

//--------------------------------------------------------------
void ofxKinectMemory::draw(){
	ofApp *app = (ofApp *)ofxGetAppPtr();
	app->cam.end();

	ofPushMatrix();
	//cam.begin();
	ofClear(0);
	ofSetColor(255);

	/*if (bShowHelp) {
	app->cam.disableMouseInput();
	} else {
	app->cam.enableMouseInput();
	}*/
	app->cam.begin();

	if (bGotImage) {
		//ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);

		if (bShowImages) {
			grayImageFiltered.draw(0,0);
			grayImage.draw(800,0);
		}



		//contourFinder.draw();
		RectTracker& tracker = contourFinder.getTracker();
		for(int i = 0; i < contourFinder.size(); i++) {

			if (bShowLabels) {
				ofPoint center = toOf(contourFinder.getCenter(i));
				ofPushMatrix();
				ofTranslate(center.x, center.y);
				int label = contourFinder.getLabel(i);
				string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
				ofDrawBitmapString(msg, 0, 0);
				ofVec2f velocity = toOf(contourFinder.getVelocity(i));
				ofScale(5, 5);
				ofLine(0, 0, velocity.x, velocity.y);
				ofPopMatrix();
			}


			vector<cv::Point> hull = contourFinder.getConvexHull(i);
			ofPolyline polyline;
			polyline.resize(hull.size());
			for(int i = 0; i < (int)hull.size(); i++) {
				polyline[i].x = hull[i].x;
				polyline[i].y = hull[i].y;
			}
			polyline.close();

			polyline.draw();

		}

	}
	//cam.end();


	app->cam.end();
	if (bShowHelp) {

		gui.draw();
	}

	app->cam.begin();
}

//--------------------------------------------------------------
void ofxKinectMemory::keyPressed(int key){
	switch (key) {
	case 'h': bShowHelp = !bShowHelp;
		break;
	case'l':
		gui.loadFromFile(GUI_SETTINGS);
		kinect.setCameraTiltAngle(angle);
		break;
	case's':
		gui.saveToFile(GUI_SETTINGS);
		break;
	case 'a':
		kinect.setCameraTiltAngle(angle);
		break;
	case OF_KEY_UP:
		angle++;
		if(angle>30) angle=30;
		kinect.setCameraTiltAngle(angle);
		break;

	case OF_KEY_DOWN:
		angle--;
		if(angle<-30) angle=-30;
		kinect.setCameraTiltAngle(angle);
		break;
	}
}

//--------------------------------------------------------------
void ofxKinectMemory::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}