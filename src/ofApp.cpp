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
    
    if (IS_HOST==1)
    {
        // open an outgoing connection to HOST:PORT
        cout << "sending osc messages on " << HOST << ":" << PORT << "\n";
        sender.setup(HOST, PORT);
    } else
    {
        cout << "listening for osc messages on port " << PORT << "\n";
        receiver.setup(PORT);

    }
}

//--------------------------------------------------------------
void ofApp::setupSceneManager() {

	setDrawFramerate(false);

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


	int i = 0;
#if __APPLE__

	sceneManager.add(new ofxBgScene(false, true, false, IntToString(i++)));
	sceneManager.add(new ofxTerrain(IntToString(i++)));
	sceneManager.add(new ofxVasaDalleQuad(false, IntToString(i++)));
	sceneManager.add(new ofxVasaDalleQuad(true, IntToString(i++)));
	sceneManager.add(new ofxVasaSquareField(false, IntToString(i++)));
	sceneManager.add(new ofxVasaLianas(IntToString(i++)));
    sceneManager.add(new ofxTunnel(IntToString(i++)));
	sceneManager.add(new ofxCityPingPong(IntToString(i++)));

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

#endif
	//sceneManager.add(new ofxTerrain());
	sceneManager.setup(true); // true = setup all the scenes now (not on the fly)
	setSceneManagerUpdate(true);
	setSceneManagerDraw(false);
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
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        // check for mouse moved message
        if(m.getAddress() == "/mouse/position"){
            // both the arguments are int32's
            Globals::oscMouseX = m.getArgAsInt32(0);
            Globals::oscMouseY = m.getArgAsInt32(1);
            cout << "received mouse " << Globals::oscMouseX << endl;
        }
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
	}


	ofClear(255);
	ofEnableAntiAliasing();


	ofEnableAntiAliasing();
	cam.begin();

	ofPushMatrix();


	ofTranslate(-ofGetWidth() / 2, ofGetHeight() / 2);
	ofScale(1, -1, 1);
	sceneManager.draw();

	ofPopMatrix();


	cam.end();


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
	ofDisableAlphaBlending();
	transformer.push();

	// the warp editor is drawn automatically after this function
	//ofDisableAntiAliasing();
}

// current scene input functions are called automatically before calling these
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {


	switch (key) {

	case 'e': cam.disableMouseInput(); break;
	case 'E': cam.enableMouseInput(); break;
	case 'm': ofHideCursor(); break;
	case 'M': ofShowCursor(); break;
	case 'D':
		bDebug = !bDebug;
		break;

	case 'a':
		transformer.setAspect(!transformer.getAspect());
		break;

	case 'c':
		transformer.setCentering(!transformer.getCentering());
		break;

		//case 'm':
		//	transformer.setMirrorX(!transformer.getMirrorX());
		//	break;

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
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
  
    if (IS_HOST == 1) {
    ofxOscMessage m;
    m.setAddress("/mouse/position");
    m.addIntArg(x);
    m.addIntArg(y);
    sender.sendMessage(m, false);
    }
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
