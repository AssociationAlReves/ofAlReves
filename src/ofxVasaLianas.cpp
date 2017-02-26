#include "ofxVasaLianas.h"
#include "ofApp.h"


using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void ofxVasaLianas::setup() {

	forceWarpOff = false;

	int w = ofGetWidth();
	int h = ofGetHeight();
	int x = (ofGetWidth() - w) * 0.5;       // center on screen.
	int y = (ofGetHeight() - h) * 0.5;     // center on screen.
	bool invertWarp = false;
	if (invertWarp) {
		warper.setSourceRect(ofRectangle(0, 0, w, h));              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
		warper.setBottomLeftCornerPosition(ofPoint(x, y));             // this is position of the quad warp corners, centering the image on the screen.
		warper.setBottomRightCornerPosition(ofPoint(x + w, y));        // this is position of the quad warp corners, centering the image on the screen.
		warper.setTopLeftCornerPosition(ofPoint(x, y + h));      // this is position of the quad warp corners, centering the image on the screen.
		warper.setTopRightCornerPosition(ofPoint(x + w, y + h)); // this is position of the quad warp corners, centering the image on the screen.
	} else {
		warper.setSourceRect(ofRectangle(0, 0, w, h));              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
		warper.setTopLeftCornerPosition(ofPoint(x, y));             // this is position of the quad warp corners, centering the image on the screen.
		warper.setTopRightCornerPosition(ofPoint(x + w, y));        // this is position of the quad warp corners, centering the image on the screen.
		warper.setBottomLeftCornerPosition(ofPoint(x, y + h));      // this is position of the quad warp corners, centering the image on the screen.
		warper.setBottomRightCornerPosition(ofPoint(x + w, y + h)); // this is position of the quad warp corners, centering the image on the screen.
	}
	
	warper.setup();
	warper.load(); // reload last saved changes.
	warper.toggleShow();

	initGui();
	initLianas();
	initKinect();

}

//--------------------------------------------------------------
void ofxVasaLianas::initGui() {

	gui.setup("panel", LIANAS_SETTINGS_FILE); // most of the time you don't need a name but don't forget to call setup

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
	nodeParams.add(nodeVelocity.set("nodeVelocity", 0.99f, -1, 10));
	gui.add(nodeParams);

	springParams.setName("Springs");
	springParams.add(numNodes.set("numNodes", 20, 2, 100));
	springParams.add(springLength.set("springLength", 20, 0, 500));
	springParams.add(springStiffness.set("springStiffness", 3, 0, 10));
	springParams.add(stringDamping.set("stringDamping", 0.9, 0, 1));
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
	cvGroup.add(contourMaxArea.set("contourMaxArea", 800, 0, 640));
	cvGroup.add(blurSize.set("blurSize", 10, 0, 50));
	cvGroup.add(maximumDistance.set("maximumDistance", 32, 0, 300));
	cvGroup.add(persistence.set("persistence", 60, 0, 100));
	gui.add(cvGroup);

	debugGroup.setName("debug");
	debugGroup.add(bShowLabels.set("ShowLabels", true));
	debugGroup.add(bShowImages.set("ShowImages", true));
	debugGroup.add(angle.set("kinect angle", 0, -30, 30));
	gui.add(debugGroup);
	bShowGui = false;

	initLianas();

}

//--------------------------------------------------------------
void ofxVasaLianas::initLianas() {

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


}

//--------------------------------------------------------------
void ofxVasaLianas::initKinect() {

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
		lianas[i]->repulsionRadius = repulsionRadius;

		//
		// update
		lianas[i]->update(lockX, lockY, lockZ);
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
void ofxVasaLianas::draw() {

	ofClear(0);
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



	//cam.begin();
	for (int i = 0; i < lianas.size(); i++) {
		lianas[i]->draw();
	}
	
	drawKinect();
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

	
	//cam.end();
	stringstream ss;
	ss << "FPS : " + ofToString(ofGetFrameRate());
	ofDrawBitmapStringHighlight(ss.str(), 10, 10);
	if (bShowGui)
	{
		gui.draw();
	}
	app->cam.begin();
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
		} // for each blob
		
		
		if (bShowImages) {
			grayImageFiltered.draw(10, 10);
			grayImage.draw(800, 0);
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
void ofxVasaLianas::keyPressed(int key) {
	switch (key) {
	case 'r': initLianas(); break;
	case 'h': bShowGui = !bShowGui; break;
	case 'l': gui.loadFromFile(LIANAS_SETTINGS_FILE);
			kinect.setCameraTiltAngle(angle);
			break;
	case 's': gui.saveToFile(LIANAS_SETTINGS_FILE); break;
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
	case 'C':
		kinect.setCameraTiltAngle(0); // zero the tilt on exit
		kinect.close();
		break;
	case 'O':
		kinect.open();
		kinect.setCameraTiltAngle(angle);
		break;
				//case 'C': cam.enableMouseInput(); break;
	//case 'c': cam.disableMouseInput(); break;
	}
	if (key == 'W') {
		forceWarpOff = false;
	}
	if (key == 'w') {
		forceWarpOff = true;
	}
	if (!forceWarpOff) {
		// WARPs
		if (key == 'H') {
			warper.toggleShow();
		}
		if (key == 'L') {
			warper.load();
		}
		if (key == 'S') {
			//					camOrientation = app->cam.getOrientationEuler();
			//					camPosition = app -> cam.getPosition();
			gui.saveToFile(LIANAS_SETTINGS_FILE);
			
			warper.save();
		}
		
	}
	


	for (int i = 0; i < lianas.size(); i++) {
		lianas[i]->keyPressed(key);
	}
}

//--------------------------------------------------------------
void ofxVasaLianas::keyReleased(int key) {

	for (int i = 0; i < lianas.size(); i++) {
		lianas[i]->keyReleased(key);
	}
}

//--------------------------------------------------------------
void ofxVasaLianas::mouseMoved(int x, int y) {
	for (int i = 0; i < lianas.size(); i++) {
		lianas[i]->mouseMoved(x, y);
	}
}

//--------------------------------------------------------------
void ofxVasaLianas::mousePressed(int x, int y, int button) {
	for (int i = 0; i < lianas.size(); i++) {
		lianas[i]->mousePressed(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxVasaLianas::mouseReleased(int x, int y, int button) {
	for (int i = 0; i < lianas.size(); i++) {
		lianas[i]->mouseReleased(x, y, button);
	}
}
