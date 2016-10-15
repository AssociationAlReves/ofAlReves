#include "ofxKinectMemory.h"
#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

const string GUI_SETTINGS = "kinectmemory_settings.xml";

//--------------------------------------------------------------
void ofxKinectMemory::setup() {
	ofSetVerticalSync(true);
	ofBackground(0);
	ofEnableArbTex();

	forceWarpOff = true;
	bDrawJoinedActors = false;

	// enable depth->video image calibration
	kinect.setRegistration(true);
	kinect.init();
	kinect.open();		// opens first available kinect
	// print the intrinsic IR sensor values
	if (kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}


	bShowHelp = false;
	bKinectFrameReady = false;

	grayImage.allocate(kinect.width, kinect.height, ofImageType::OF_IMAGE_GRAYSCALE);
	grayImage.clear();

	//-----------------------------------------
	// FBOs

	fboWhite.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGBA32F_ARB);
	fboBlack.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGBA32F_ARB);
	/*
		fboWhite.allocate(kinect.width*2, kinect.height*2, GL_RGBA32F_ARB);
		fboBlack.allocate(kinect.width*2, kinect.height*2, GL_RGBA32F_ARB);*/

	fboWhite.begin();
	ofClear(255, 255, 255, 0);
	fboWhite.end();

	fboBlack.begin();
	ofClear(255, 255, 255, 0);
	fboBlack.end();
	//-----------------------------------------

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
	gui.setup("Memory", GUI_SETTINGS);

	cvGroup.setName("OpenCV");
	cvGroup.add(nearThreshold.set("nearThreshold", 255, 0, 255));
	cvGroup.add(farThreshold.set("farThreshold", 213, 0, 255));
	cvGroup.add(thresholdParam.set("threshold", 13, 0, 255));
	cvGroup.add(contourMinArea.set("contourMinArea", 1, 0, 640));
	cvGroup.add(contourMaxArea.set("contourMaxArea", 800, 0, 640));
	cvGroup.add(blurSize.set("blurSize", 10, 0, 50));
	cvGroup.add(maximumDistance.set("maximumDistance", 32, 0, 300));
	cvGroup.add(persistence.set("persistence", 15, 0, 100));
	gui.add(cvGroup);

	appGroup.setName("App");
	appGroup.add(numFramesDelay.set("numFramesDelay", 35, 1, 200));
	appGroup.add(angle.set("kinect angle", 13, -30, 30));
	appGroup.add(bStartMemory.set("StartMemory", false));
	appGroup.add(fadeAmnt.set("fade amount", 10, 0, 50));
	appGroup.add(blackScreen.set("blackScreen", true));
	appGroup.add(antiAlias.set("antiAlias", true));
	appGroup.add(lineWidth.set("lineWidth", 1, 0, 10));
	//appGroup.add(lineColor.set("lineColor", ofColor(0), ofColor(0), ofColor(255)));

	gui.add(appGroup);

	debugGroup.setName("debug");
	debugGroup.add(bShowLabels.set("ShowLabels", true));
	debugGroup.add(bShowImages.set("ShowImages", true));
	gui.add(debugGroup);


	//if (!forceWarpOff) {
		// WARP
		int w = ofGetWidth();
		int h = ofGetHeight();
		int x = (ofGetWidth() - w) * 0.5;       // center on screen.
		int y = (ofGetHeight() - h) * 0.5;     // center on screen.
		warper.setSourceRect(ofRectangle(0, 0, w, h));              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
		warper.setTopLeftCornerPosition(ofPoint(x, y));             // this is position of the quad warp corners, centering the image on the screen.
		warper.setTopRightCornerPosition(ofPoint(x + w, y));        // this is position of the quad warp corners, centering the image on the screen.
		warper.setBottomLeftCornerPosition(ofPoint(x, y + h));      // this is position of the quad warp corners, centering the image on the screen.
		warper.setBottomRightCornerPosition(ofPoint(x + w, y + h)); // this is position of the quad warp corners, centering the image on the screen.
		warper.setup();
		warper.load(); // reload last saved changes.
	//}

}

//--------------------------------------------------------------
void ofxKinectMemory::update() {

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
			grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height, ofImageType::OF_IMAGE_GRAYSCALE, false);
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
void ofxKinectMemory::draw() {

	ofApp *app = (ofApp *)ofxGetAppPtr();

	if (!forceWarpOff) {
		//=======================================
		// WARP		
		app->cam.end();
		//======================== get our quad warp matrix.
		ofMatrix4x4 mat = warper.getMatrix();
		//======================== use the matrix to transform our fbo.
		app->cam.begin();
		glPushMatrix();
		glMultMatrixf(mat.getPtr());
		// ====================================
	}

	ofPushMatrix();

	ofScale(1, -1, 1);
	ofTranslate(0, -ofGetScreenHeight());
	//cam.begin();
	//ofClear(0);
	ofSetColor(255);


	if (bKinectFrameReady) {

		RectTracker& tracker = contourFinder.getTracker();

		// delete dead actors
		for (auto & label : tracker.getDeadLabels())
		{
			//cout << "Dead actor: " << label << endl;
			actors.erase(label);
			actorsHullUnion.erase(label);
		}
		// delete new actors
		for (auto & label : tracker.getNewLabels())
		{
			//cout << "New actor: " << label << endl;
			actors[label] = list<vector<cv::Point> >();
		}


		// union of all points
		vector<cv::Point> mergedHullsTotal;
		vector<cv::Point> HullTotal;

		// for each blob
		for (int i = 0; i < contourFinder.size(); i++) {

			int label = contourFinder.getLabel(i);

			vector<cv::Point> hullPoints = contourFinder.getConvexHull(i);
			if (bStartMemory) {

				list<vector<cv::Point> >& actor = actors[label];
				// add polyline
				if (actor.size() == 0) {
					actor.assign(numFramesDelay, hullPoints);
				}
				else {
					actor.push_back(hullPoints);
				}

				// union of all points
				vector<cv::Point> mergedHulls;
				for (auto & curHull : actor) {
					mergedHulls.insert(mergedHulls.end(), curHull.begin(), curHull.end());
					mergedHullsTotal.insert(mergedHullsTotal.end(), curHull.begin(), curHull.end());
				}

				// remove oldest hull for current actor
				actor.pop_front();
				vector<cv::Point> hull;
				convexHull(mergedHulls, hull);

				ofPolyline polyline;
				polyline.resize(hull.size());
				for (int hullIndex = 0; hullIndex < (int)hull.size(); hullIndex++) {
					polyline[hullIndex].x = hull[hullIndex].x;
					polyline[hullIndex].y = hull[hullIndex].y;
				}
				polyline.close();
				actorsHullUnion[label] = polyline;
				/*
				ofSetColor(ofColor::blue);
				polyline.draw();
				ofSetColor(255);*/


			}
			else {
				ofPolyline polyline;
				polyline.resize(hullPoints.size());
				for (int hullIndex = 0; hullIndex < (int)hullPoints.size(); hullIndex++) {
					polyline[hullIndex].x = hullPoints[hullIndex].x;
					polyline[hullIndex].y = hullPoints[hullIndex].y;
				}
				polyline.close();
				polyline.draw();

				if (bShowLabels) {
					ofPoint center = toOf(contourFinder.getCenter(i));
					ofPushMatrix();
					ofTranslate(center.x, center.y);
					string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
					ofDrawBitmapStringHighlight(msg, 0, 0, ofColor::white, ofColor::red);
					ofVec2f velocity = toOf(contourFinder.getVelocity(i));
					ofScale(5, 5);
					ofLine(0, 0, velocity.x, velocity.y);

					ofPopMatrix();
				}
			} //if (bStartMemory) 
		} // for each blob

		if (bStartMemory && mergedHullsTotal.size() > 0) {
			convexHull(mergedHullsTotal, HullTotal);
			bigHull2Actors.resize(HullTotal.size());
			for (int hullIndex = 0; hullIndex < (int)HullTotal.size(); hullIndex++) {
				bigHull2Actors[hullIndex].x = HullTotal[hullIndex].x;
				bigHull2Actors[hullIndex].y = HullTotal[hullIndex].y;
			}
			bigHull2Actors.close();
		}
	} // END if KINECT		

	//if (bGotImage)
	//cam.end();

	if (kinect.isConnected()) {
		if (bShowImages && !bStartMemory) {
			grayImageFiltered.draw(10, 10);
			grayImage.draw(800, 0);
		}
		//ofDrawAxis(50);
	}

	drawMemoryTrails();

	ofPopMatrix();

	if (!forceWarpOff) {
		//================
		// WARP DRAW
		glPopMatrix();
		app->cam.end();
		//======================== draw quad warp ui.
		ofSetColor(ofColor::magenta);
		warper.drawQuadOutline();
		ofSetColor(ofColor::yellow);
		warper.drawCorners();
		ofSetColor(ofColor::magenta);
		warper.drawHighlightedCorner();
		ofSetColor(ofColor::red);
		warper.drawSelectedCorner();
		//======================== info.
	}
	else {
		app->cam.end();
	}

	if (bShowHelp) {

		gui.draw();
	}

	app->cam.begin();
}

//--------------------------------------------------------------
void ofxKinectMemory::drawMemoryTrails() {

	if (kinect.isConnected()) {

		if (bStartMemory) {

			ofEnableAlphaBlending();
			if (antiAlias) {
				ofEnableAntiAliasing();
			}
			else {
				ofDisableAntiAliasing();
			}
			ofSetLineWidth(lineWidth);
			if (blackScreen) {
				// BLACK
				//--------------------------------------------------------------
				ofBackground(0);
				fboBlack.begin();
				ofPushMatrix();
				ofScale(2, 2);
				ofSetColor(0, 0, 0, fadeAmnt);
				ofFill();
				ofRect(0, 0, 0, fboWhite.getWidth(), fboWhite.getHeight());
				ofNoFill();
				ofSetColor(255);
				if (bDrawJoinedActors) {
					bigHull2Actors.draw();
				}
				else {
					for (auto & actor : actorsHullUnion) {
						actor.second.draw();
					}
				}

				ofPopMatrix();
				fboBlack.end();

				ofDisableAlphaBlending();

				ofPushMatrix();
				ofTranslate(fboWhite.getWidth() / 2, fboWhite.getHeight() / 2);
				ofScale(1, -1, 1);
				ofTranslate(-fboWhite.getWidth() / 2, -fboWhite.getHeight() / 2);
				//ofDisableAlphaBlending();
				fboBlack.draw(0, 0);
				ofPopMatrix();

			}
			else {
				// WHITE
				//ofBackground(255);

				ofBackground(255, 255, 255, 0);

				if (ofGetKeyPressed('c')) {
					ofClear(255, 255, 255, 0);
				}

				fboWhite.begin();
				if (ofGetKeyPressed('c')) {
					ofClear(255, 255, 255, 0);
				}
				ofPushMatrix();
				float ratioW = ofGetScreenWidth() / kinect.width;
				float ratioH = ofGetScreenHeight() / kinect.height;
				ofScale(ratioW, ratioH);

				ofSetColor(255, 255, 255, fadeAmnt);
				ofFill();
				ofRect(0, 0, 0, fboWhite.getWidth(), fboWhite.getHeight());

				ofNoFill();
				//ofSetColor(lineColor);
				ofSetColor(0);
				if (bDrawJoinedActors) {
					bigHull2Actors.draw();
				}
				else {
					for (auto & actor : actorsHullUnion) {
						actor.second.draw();
					}
				}

				ofPopMatrix();
				fboWhite.end();
				ofPushMatrix();
				ofTranslate(fboWhite.getWidth() / 2, fboWhite.getHeight() / 2);
				ofScale(1, -1, 1);
				ofTranslate(-fboWhite.getWidth() / 2, -fboWhite.getHeight() / 2);
				//ofDisableAlphaBlending();
				fboWhite.draw(0, 0);
				ofPopMatrix();
			}
		}
	}
	else
	{
		ofEnableAlphaBlending();
		// NO KINECT
		ofBackground(255, 255, 255, 0);

		fboWhite.begin();
		ofPushMatrix();
		float ratioW = ofGetScreenWidth() / kinect.width;
		float ratioH = ofGetScreenHeight() / kinect.height;
		ofScale(ratioW, ratioH);
		ofSetColor(255, 255, 255, fadeAmnt);
		ofFill();
		ofRect(0, 0, 0, fboWhite.getWidth(), fboWhite.getHeight());

		ofNoFill();
		//ofSetColor(lineColor);
		ofSetColor(0);
		ofRect(0, 0, 640, 480);
		ofCircle(0, 0, 0, 50);
		ofCircle(640, 0, 0, 50);
		ofCircle(640, 480, 0, 50);
		ofCircle(0, 480, 0, 50);

		ofPopMatrix();
		fboWhite.end();
		ofPushMatrix();
		ofTranslate(fboWhite.getWidth() / 2, fboWhite.getHeight() / 2);
		ofScale(1, -1, 1);
		ofTranslate(-fboWhite.getWidth() / 2, -fboWhite.getHeight() / 2);
		//ofDisableAlphaBlending();
		fboWhite.draw(0, 0);
		ofPopMatrix();
		ofDisableAlphaBlending();
	}
}

//--------------------------------------------------------------
void ofxKinectMemory::keyPressed(int key) {
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
		if (angle > 30) angle = 30;
		kinect.setCameraTiltAngle(angle);
		break;
	case OF_KEY_DOWN:
		angle--;
		if (angle < -30) angle = -30;
		kinect.setCameraTiltAngle(angle);
		break;
	case 'b':
		blackScreen = !blackScreen;
		break;
	case ' ':
		bStartMemory = !bStartMemory;
		break;
	case 'j':
		bDrawJoinedActors = false;
		break;
	case 'J':
		bDrawJoinedActors = true;
	}

	if (key == 'W') {
		forceWarpOff = false;
	} 
	if (key == 'w') {
		forceWarpOff = true;
	}
	if (!forceWarpOff) {
		// WARP
		if (key == 'H') {
			warper.toggleShow();
		}
		if (key == 'L') {
			warper.load();
		}
		if (key == 'S') {
			warper.save();
		}
		
	}
}

//--------------------------------------------------------------
void ofxKinectMemory::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}