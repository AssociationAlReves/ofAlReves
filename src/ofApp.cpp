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

	post.init(ofGetWidth(), ofGetHeight());
	post.createPass<FxaaPass>();
	post.createPass<FxaaPass>();
	post.createPass<BloomPass>();

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

	//// Vasarely (Mac or PC)
	//sceneManager.add(new ofxTerrain());
	//sceneManager.add(new ofxVasaDalleQuad());
	//sceneManager.add(new ofxVasaSquareField());

#if __APPLE__
	int i = 0;
//
//    /*  Katia
//     *  Spectacle A1
//     *  Vendredi 08/04/16
//     */
//    sceneManager.add(new ofxBgScene(IntToString(i++)));														postEnabledByScene[i-1] = false;
//    sceneManager.add(new ofxVideoScene("Around.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
//    sceneManager.add(new ofxVasaDalleQuad(false,IntToString(i++)));											postEnabledByScene[i-1] = false;
//    sceneManager.add(new ofxKinecticon(IntToString(i++)));										postEnabledByScene[i-1] = false;
//    
//    /*  Katia
//     *  Spectacle A2
//     *  Vendredi 08/04/16
//     */
//    sceneManager.add(new ofxVideoScene("lamer-long.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
//    sceneManager.add(new ofxVideoScene("russie.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
//    sceneManager.add(new ofxVideoScene("savane.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
//    sceneManager.add(new ofxVasaSquareField(true, IntToString(i++)));                           postEnabledByScene[i-1] = false;
//    sceneManager.add(new ofxKinecticon(IntToString(i++)));										postEnabledByScene[i-1] = false;
    
    /*  Katia
     *  Spectacle B
     *  Vendredi 14/04/16
     */
        sceneManager.add(new ofxBgScene(IntToString(i++)));														postEnabledByScene[i-1] = false;
   sceneManager.add(new ofxVideoScene("Around.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
    sceneManager.add(new ofxVideoScene("oiseaux1.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
    sceneManager.add(new ofxVideoScene("oiseaux2.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
    sceneManager.add(new ofxVideoScene("oiseaux3.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
    sceneManager.add(new ofxVideoScene("oiseaux4.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
    sceneManager.add(new ofxVasaDalleQuad(false,IntToString(i++)));											postEnabledByScene[i-1] = false;
    

    /*
    // Vasarely (mac - front with kinect)
	
	sceneManager.add(new ofxBgScene(IntToString(i++)));														postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxTerrain(IntToString(i++)));														postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxVasaDalleQuad(false,IntToString(i++)));											postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxVasaSquareField(false, IntToString(i++)));												postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxKinectListener(IntToString(i++)));												postEnabledByScene[i-1] = false;
    sceneManager.add(new ofxKinecticon(IntToString(i++)));												postEnabledByScene[i-1] = false;
    sceneManager.add(new ofxVasaCubeMesh(IntToString(i++)));                                            postEnabledByScene[i-1] = false;
    */
    
    /*
     // Bellegarde (mac - front with kinect)
     int i = 0;
     sceneManager.add(new ofxVideoScene("Sepio Ink in Water redux.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
     sceneManager.add(new ofxCrossedLines(true, IntToString(i++)));											postEnabledByScene[i-1] = false;
     sceneManager.add(new ofMovingSquares(IntToString(i++), true));											postEnabledByScene[i-1] = false;
     sceneManager.add(new ofxVideoScene("Around.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
     sceneManager.add(new ofxVideoScene("Light Bulbs.mov", IntToString(i++), true, false, false));			postEnabledByScene[i-1] = false;
     sceneManager.add(new ofxSquareScreen(IntToString(i++), true));											postEnabledByScene[i-1] = true;
     sceneManager.add(new ofxVasaSquareField(IntToString(i++)));												postEnabledByScene[i-1] = false;
     sceneManager.add(new ofxKinecticon(IntToString(i++)));													postEnabledByScene[i-1] = false;
     sceneManager.add(new ofxRibbon(IntToString(i++)));
     */
#else
	int i = 0;
	// Bellegarde (PC - top without kinect)

	sceneManager.add(new ofxBgScene(false,IntToString(i++)));												postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxCity(IntToString(i++)));														postEnabledByScene[i-1] = false;

	/*sceneManager.add(new ofxVideoScene("Sepio Ink in Water redux.mov", IntToString(i++), false, false, true));	postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxCrossedLines(false, IntToString(i++)));											postEnabledByScene[i-1] = false;
	sceneManager.add(new ofMovingSquares(IntToString(i++), false));											postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxVideoScene("Around.mov", IntToString(i++), false, false, false));	postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxBgScene(IntToString(i++)));														postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxVideoScene("Light Bulbs.mov", IntToString(i++), true, false, false));			postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxRibbon(IntToString(i++)));														postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxSquareScreen(IntToString(i++),false));											postEnabledByScene[i-1] = true;
	sceneManager.add(new ofxVasaDalleQuad(true,IntToString(i++)));											postEnabledByScene[i-1] = false;
	sceneManager.add(new ofxVasaSquareField(IntToString(i++), VASA_SQUARE_MODE_FULL_RND_ROTATION));			postEnabledByScene[i-1] = false;*/
	

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

}

string ofApp::IntToString(int i) {
	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << i;
	return oss.str();
}

//--------------------------------------------------------------
void ofApp::update() {

	// the current scene is automatically updated before this function
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofClear(255);
	ofEnableAntiAliasing();

	if (postEnabledByScene[sceneManager.getCurrentSceneIndex()]) {
		post.begin(cam);
	} else {
		ofEnableAntiAliasing();
		cam.begin();
	}

	ofPushMatrix();


	ofTranslate(-ofGetWidth()/2, ofGetHeight()/2);
	ofScale (1,-1,1);
	// the current scene is automatically drawn before this function
	sceneManager.draw();

	ofPopMatrix();
	if (postEnabledByScene[sceneManager.getCurrentSceneIndex()]) {
		post.end();
	} else {
		cam.end();
	}

	// drop out of the auto transform space back to OF screen space
	transformer.pop();

	// draw the transform panel when in debug mode
	if(isDebug()) {
		// draw current scene info using the ofxBitmapString stream interface
		// to ofDrawBitmapString
		ofSetColor(200);
		ofxBitmapString(12, ofGetHeight()-8)
			<< "Current Scene: " << sceneManager.getCurrentSceneIndex()
			<< " " << sceneManager.getCurrentSceneName() << endl;
	}

	// go back to the auto transform space
	//
	// this is actually done automatically if the transformer is set but
	// included here for completeness
	transformer.push();

	// the warp editor is drawn automatically after this function
	//ofDisableAntiAliasing();
}

// current scene input functions are called automatically before calling these
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

    
	switch (key) {

	case 'e': cam.disableMouseInput(); break;
	case 'E' : cam.enableMouseInput(); break;
	case 'm' : ofHideCursor(); break;
	case 'M' : ofShowCursor(); break;
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
	case OF_KEY_BACKSPACE:
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
