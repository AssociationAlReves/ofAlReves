/*
* ofxAppUtils
* Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
*
* BSD Simplified License.
* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*
* See https://github.com/danomatika/ofxAppUtils for documentation
*
*/
#include "ofApp.h"

#include "scenes.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// setup for nice jaggy-less rendering
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	ofBackground(0, 0, 0);

	setupSceneManager();

}

//--------------------------------------------------------------
void ofApp::setupSceneManager() {

	// setup the render size (working area)
	transformer.setRenderSize(PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y);

	// turn on transform origin translation and scaling to screen size,
	// disable quad warping, and enable aspect ratio and centering when scaling
	transformer.setTransforms(true, true, false, true, true);

	// set the ofxApp transformer so it's automatically applied in draw()
	setTransformer(&transformer);

#ifdef HAVE_OFX_GUI
	// setup transform panel with transformer pointer,
	// loads settings & quad warper xml files if found
	panel.setup(&transformer);
#endif

	// load scenes

	//// Vasarely (Mac or PC)
	//sceneManager.add(new ofxTerrain());
	//sceneManager.add(new ofxVasaDalleQuad());
	//sceneManager.add(new ofxVasaSquareField());

#if __APPLE__
	// Bellegarde (mac - front with kinect)
	int i = 0;
	inkScene = (ofxVideoScene*) sceneManager.add(new ofxVideoScene("Sepio Ink in Water.mov", IntToString(i++)));
	inkScene->horizontalFlip = false;
	sceneManager.add(new ofxCrossedLines(true, IntToString(i++)));
	sceneManager.add(new ofMovingSquares());
	sceneManager.add(new ofxVideoScene("Light Bulbs.mov", IntToString(i++)));
	squareScreen = (ofxSquareScreen*) sceneManager.add(new ofxSquareScreen(IntToString(i++))); // save pointer
	squareScreen->openFromBottom = true;
	sceneManager.add(new ofxVasaDalleQuad(true, IntToString(i++)));
	sceneManager.add(new ofxVasaSquareField(IntToString(i++)));
	sceneManager.add(new ofxKinecticon(IntToString(i++)));	
#else
	int i = 0;
	// Bellegarde (PC - top without kinect)
	inkScene = (ofxVideoScene*) sceneManager.add(new ofxVideoScene("Sepio Ink in Water.mov", IntToString(i++), false));
	inkScene->horizontalFlip = true;	
	sceneManager.add(new ofxCrossedLines(false, IntToString(i++)));
	sceneManager.add(new ofMovingSquares(IntToString(i++)));
	squareScreen = (ofxSquareScreen*) sceneManager.add(new ofxSquareScreen(IntToString(i++))); // save pointer
	squareScreen->openFromBottom = false;
	sceneManager.add(new ofxVasaDalleQuad(true,IntToString(i++)));
	sceneManager.add(new ofxVasaSquareField(IntToString(i++)));
	
#endif
	//sceneManager.add(new ofxTerrain());
	sceneManager.setup(true); // true = setup all the scenes now (not on the fly)
	setSceneManagerUpdate(true);
	setSceneManagerDraw(false);
	ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside

	// start with a specific scene
	// set now to true in order to ignore the scene fade and change now
	sceneManager.gotoScene("SquareScreen", true);
	lastScene = sceneManager.getCurrentSceneIndex();

	// overlap scenes when transitioning
	sceneManager.setOverlap(true);

	// attach scene manager to this ofxApp so it's called automatically,
	// you can also call the callbacks (update, draw, keyPressed, etc) manually
	// if you don't set it
	//
	// you can also turn off the auto sceneManager update and draw calls with:
	// setSceneManagerUpdate(false);
	// setSceneManagerDraw(false);
	//
	// the input callbacks in your scenes will be called if they are implemented
	//
	setSceneManager(&sceneManager);

}

string ofApp::IntToString(int i) {
	std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << i;
    return oss.str();
}

//--------------------------------------------------------------
void ofApp::update() {

	// the current scene is automatically updated before this function

#ifdef HAVE_OFX_GUI
	// update the transform panel when in debug mode
	if(isDebug()) {
		panel.update();
	}
#endif
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofClear(255);
	ofEnableAntiAliasing();
	cam.begin();
	ofPushMatrix();

	
	ofTranslate(-ofGetWidth()/2, ofGetHeight()/2);
	ofScale (1,-1,1);
	// the current scene is automatically drawn before this function
	sceneManager.draw();

	ofPopMatrix();
	cam.end();

	// show the render area edges with a white rect
	if(isDebug()) {
		ofNoFill();
		ofSetColor(255);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofRect(1, 1, getRenderWidth()-2, getRenderHeight()-2);
		ofFill();
	}

	// drop out of the auto transform space back to OF screen space
	transformer.pop();

#ifdef HAVE_OFX_GUI
	// draw the transform panel when in debug mode
	if(isDebug()) {
		panel.draw();
	}
#endif

	// draw current scene info using the ofxBitmapString stream interface
	// to ofDrawBitmapString
	ofSetColor(200);
	ofxBitmapString(12, ofGetHeight()-8)
		<< "Current Scene: " << sceneManager.getCurrentSceneIndex()
		<< " " << sceneManager.getCurrentSceneName() << endl;

	// go back to the auto transform space
	//
	// this is actually done automatically if the transformer is set but
	// included here for completeness
	transformer.push();

	// the warp editor is drawn automatically after this function
	ofDisableAntiAliasing();
}

// current scene input functions are called automatically before calling these
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key) {

	case 'd': {
		//
		cout << "pos: " << cam.getPosition() << endl;
		cout << "ori: " << cam.getOrientationEuler() << endl;
		//float o = cam.getOrientationEuler().x;
		cam.setOrientation(ofVec3f(43.1984,0,0));
		cam.setPosition(-200, -200.867, 208.106);

			  } break;

	case 'D':
		bDebug = !bDebug;
		break;

	case 'a':
		transformer.setAspect(!transformer.getAspect());
		break;

	case 'c':
		transformer.setCentering(!transformer.getCentering());
		break;

	case 'm':
		transformer.setMirrorX(!transformer.getMirrorX());
		break;

	case 'n':
		transformer.setMirrorY(!transformer.getMirrorY());
		break;

	case 'q':
		transformer.setWarp(!transformer.getWarp());
		break;

	case 'f':
		ofToggleFullscreen();
		break;

	case ')':
		sceneManager.prevScene();
		break;

	case '=':
		sceneManager.nextScene();
		break;

	case OF_KEY_F1: sceneManager.gotoScene(0); break;
	case OF_KEY_F2: sceneManager.gotoScene(1);  break;
	case OF_KEY_F3: sceneManager.gotoScene(2);  break;
	case OF_KEY_F4: sceneManager.gotoScene(3);  break;
	case OF_KEY_F5: sceneManager.gotoScene(4);  break;
	case OF_KEY_F6: sceneManager.gotoScene(5);  break;
	case OF_KEY_F7: sceneManager.gotoScene(6);  break;
	case OF_KEY_F8: sceneManager.gotoScene(7);  break;
	case OF_KEY_F9:  sceneManager.gotoScene(8); break;
	case OF_KEY_F10: sceneManager.gotoScene(9); break;

	case OF_KEY_BACKSPACE:
		if(sceneManager.getCurrentScene()) { // returns NULL if no scene selected
			lastScene = sceneManager.getCurrentSceneIndex();
		}
		sceneManager.noScene();
		break;

	case 'o':
		sceneManager.setOverlap(!sceneManager.getOverlap());
		break;

	case OF_KEY_LEFT:
		{
			ofVec3f euler = cam.getOrientationEuler();
			euler.x -= 0.1;
			cam.setOrientation(euler);

		}
		break;
	case OF_KEY_RIGHT:
		{
			ofVec3f euler = cam.getOrientationEuler();
			euler.x += 0.1;
			cam.setOrientation(euler);

		}
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

	// transformer.setNewScreenSize() is automatically called if the transformer is set

}
