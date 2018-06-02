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
    osc.setup();

	ofBackground(0, 0, 0);

	setupSceneManager();

    Globals::screenWidth = ofGetWidth();
    Globals::screenHeight = ofGetHeight();

}

//--------------------------------------------------------------
void ofApp::setupSceneManager() {

	setDrawFramerate(false);

	// setup the render size (working area)
	transformer.setRenderSize(PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y);

	// turn on transform origin translation and scaling to screen size,
	// disable quad warping, and enable aspect ratio and centering when scaling
	transformer.setTransforms(false, false, false, false, false);

	// set the ofxApp transformer so it's automatically applied in draw()
	setTransformer(&transformer);

#ifdef HAVE_OFX_GUI
	// setup transform panel with transformer pointer,
	// loads settings & quad warper xml files if found
	panel.setup(&transformer);
#endif

	// load scenes


	int i = 0;
#if __APPLE__

    sceneManager.add(new ofxBgScene(false, true, false, IntToString(i++)));
    sceneManager.add(new ofxTerrain(IntToString(i++)));
    sceneManager.add(new ofxVasaDalleQuad(false, IntToString(i++)));
    sceneManager.add(new ofxVasaDalleQuad(true, IntToString(i++)));
    sceneManager.add(new ofxVasaSquareField(false, IntToString(i++)));
    sceneManager.add(new ofxVasaLianas(IntToString(i++)));
    sceneManager.add(new ofParticles(IntToString(i++)));
    sceneManager.add(new ofxCityPingPong(IntToString(i++)));
    sceneManager.add(new ofxKinecticon(IntToString(i++)));
    sceneManager.add(new ofxVideoMirror(IntToString(i++)));
    sceneManager.add(new ofxRibbon(IntToString(i++)));

	//	// Bellegarde (spectacle + ateliers Vasarely)
	//	sceneManager.add(new ofxBgScene(false, true, false, IntToString(i++)));
	//	sceneManager.add(new ofxCity(IntToString(i++)));
	//	sceneManager.add(new ofxCityPingPong(IntToString(i++)));
	//	sceneManager.add(new ofxBgScene(false, false, true, IntToString(i++)));
	//	sceneManager.add(new ofxKinectMemory(IntToString(i++)));
	//	sceneManager.add(new ofxKinecticon(IntToString(i++)));
	//	sceneManager.add(new ofxTerrain(IntToString(i++)));
	//	sceneManager.add(new ofxVasaDalleQuad(false, IntToString(i++)));
	//	sceneManager.add(new ofxVasaSquareField(false, IntToString(i++)));

#else

	// Bellegarde (spectacle + ateliers Vasarely)
	sceneManager.add(new ofxBgScene(false, true, false, IntToString(i++)));
	sceneManager.add(new ofxTerrain(IntToString(i++)));
	sceneManager.add(new ofxVasaDalleQuad(false, IntToString(i++)));
	sceneManager.add(new ofxVasaSquareField(false, IntToString(i++)));
	sceneManager.add(new ofxVasaLianas(IntToString(i++)));
    sceneManager.add(new ofxTunnel(IntToString(i++)));
	sceneManager.add(new ofxCityPingPong(IntToString(i++)));
    sceneManager.add(new ofxKinecticon(IntToString(i++)));

#endif
	//sceneManager.add(new ofxTerrain());
	sceneManager.setup(true); // true = setup all the scenes now (not on the fly)

	ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside

	// start with a specific scene
	// set now to true in order to ignore the scene fade and change now
	sceneManager.noScene();

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

	stringstream ss;
	ss << "Registered scenes !" << endl;
	for (int i = 0; i < sceneManager.getNumScenes(); i++) {
		ss << "F" << i + 1 << ": " << sceneManager.getSceneName(i) << endl;
	}
	cout << ss.str() << endl;;

}

string ofApp::IntToString(int i) {
	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << i;
	return oss.str();
}

//--------------------------------------------------------------
void ofApp::update() {

	// the current scene is automatically updated before this function
	// update the transform panel when in debug mode
	if (isDebug()) {
		panel.update();
	}

    bool isdbg = isDebug();
    if (isDebug()){
        osc.update(isdbg);
    }
    
}


//--------------------------------------------------------------
void ofApp::draw() {

	if (isDebug()) {
		ofNoFill();
		ofSetColor(255);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofDrawRectangle(1, 1, getRenderWidth() - 2, getRenderHeight() - 2);
		ofFill();
        
        osc.draw();
	}

	// drop out of the auto transform space back to OF screen space
	transformer.pop();

	ofEnableAlphaBlending();
#ifdef HAVE_OFX_GUI
	// draw the transform panel when in debug mode
	if (isDebug()) {
		panel.draw();
	}
#endif

	// draw the transform panel when in debug mode
	if (isDebug()) {
		// draw current scene info using the ofxBitmapString stream interface
		// to ofDrawBitmapString
		ofSetColor(200);
		ofxBitmapString(12, ofGetHeight() - 8)
			<< "Current Scene: " << sceneManager.getCurrentSceneIndex()
			<< " " << sceneManager.getCurrentSceneName() << endl;
	}

	// go back to the auto transform space
	//
	// this is actually done automatically if the transformer is set but
	// included here for completeness

    //ofDisableAlphaBlending();
	transformer.push();

	// the warp editor is drawn automatically after this function
	//ofDisableAntiAliasing();
}

// current scene input functions are called automatically before calling these
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

    osc.keyPressed(key);

	switch (key) {

        case 'e' :
            transformer.disableEasyCamMouseInput();
            break;
        case 'E' :
            transformer.enableEasyCamMouseInput();
            break;


	case 'm': ofHideCursor(); break;
	case 'M': ofShowCursor(); break;
	case 'D':
		bDebug = !bDebug;
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
    case 'w':
        transformer.setWarp(false);
        break;
    case 'W':
        transformer.setWarp(true);
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
	case OF_KEY_F11: sceneManager.gotoScene(10); break;
	case OF_KEY_F12: sceneManager.gotoScene(11); break;
	case OF_KEY_BACKSPACE:
		sceneManager.noScene();
		break;

	case 'o':
		sceneManager.setOverlap(!sceneManager.getOverlap());
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    osc.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    osc.mouseMoved(x, y);
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

    Globals::screenWidth = w;
    Globals::screenHeight = h;
	// transformer.setNewScreenSize() is automatically called if the transformer is set

}
