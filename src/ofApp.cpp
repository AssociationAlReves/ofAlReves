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
//    // F1 Black
//    sceneManager.add(new ofxBgScene(false, false, true, IntToString(i++)));
//    // F2 White
//    sceneManager.add(new ofxBgScene(false, false, false, IntToString(i++)));
//    // F3 Hot
//    sceneManager.add(new ofxBgScene(false, true, false, IntToString(i++)));
//    // F4 particles
//    sceneManager.add(new ofParticles(IntToString(i++)));
//    // F5 ribbon
//    sceneManager.add(new ofxRibbon(IntToString(i++)));
//    // F6 4 colors
//    sceneManager.add(new ofxVideoScene("vid/couleurs.mp4", IntToString(i++), true, true, true, true));
//    // F7 Marilyn
//    sceneManager.add(new ofMarilyn(IntToString(i++)));
//    // F8 popup
//    sceneManager.add(new ofxVisualPopupManager(IntToString(i++), COMIC_STRIP, POPUP));
//    // F9 popup Moulinex
//    sceneManager.add(new ofxVideoScene("vid/MixPub.mp4", IntToString(i++), true, false, false, false));
//    // F10 supermarket
//    sceneManager.add(new ofxVisualPopupManager(IntToString(i++), SUPERMARKETLADY, NONE));
//    // F11 bonbons
//    sceneManager.add(new ofxVisualPopupManager(IntToString(i++), BONBONS, NONE));
//    //sceneManager.add(new ofxVisualPopupManager(IntToString(i++), MOULINEX, NONE));
    
    
    // Bellegarde (spectacle + ateliers Vasarely)
    sceneManager.add(new ofxBgScene(false, true, false, IntToString(i++)));
    sceneManager.add(new ofxTerrain(IntToString(i++)));
    sceneManager.add(new ofxVasaDalleQuad(false, IntToString(i++)));
    sceneManager.add(new ofxVasaDalleQuad(true, IntToString(i++)));
    sceneManager.add(new ofxVasaSquareField(false, IntToString(i++)));
    sceneManager.add(new ofxVasaLianas(IntToString(i++)));
    sceneManager.add(new ofParticles(IntToString(i++)));
    //sceneManager.add(new ofxCityPingPong(IntToString(i++)));
    sceneManager.add(new ofxKinecticon(IntToString(i++)));
    //sceneManager.add(new ofxVideoMirror(IntToString(i++)));
    //	sceneManager.add(new ofMovingSquares(IntToString(i++)));
    sceneManager.add(new ofxRibbon(IntToString(i++)));
    
    sceneManager.setup(false); // true = setup all the scenes now (not on the fly)
    
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
    
    transformer.cam.enableInertia();
    transformer.cam.setDrag(0.9); // 0.9 is default
    
    stringstream ss;
    ss << "Registered scenes !" << endl;
    for (int i = 0; i < sceneManager.getNumScenes(); i++) {
        ss << "F" << i + 1 << ": " << sceneManager.getSceneName(i) << endl;
    }
    ss << endl << "cam inertia enabled: " << transformer.cam.getInertiaEnabled() << endl;
    ss << endl << "cam drag: " << transformer.cam.getDrag() << endl;
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
    
#ifdef ALREVES_USE_OSC
    
    osc.update(isDebug());
    
    if (sceneManager.getCurrentSceneIndex() != Globals::oscSceneIndex
        && Globals::oscSceneIndex >= 0){
        sceneManager.gotoScene(Globals::oscSceneIndex);
    }
    
#endif
    
    
    
}


//--------------------------------------------------------------
void ofApp::draw() {
    ofEnableAlphaBlending();
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
            
        case OF_KEY_F1: switchSceneManagerScene(0); break;
        case OF_KEY_F2: switchSceneManagerScene(1);  break;
        case OF_KEY_F3: switchSceneManagerScene(2);  break;
        case OF_KEY_F4: switchSceneManagerScene(3);  break;
        case OF_KEY_F5: switchSceneManagerScene(4);  break;
        case OF_KEY_F6: switchSceneManagerScene(5);  break;
        case OF_KEY_F7: switchSceneManagerScene(6);  break;
        case OF_KEY_F8: switchSceneManagerScene(7);  break;
        case OF_KEY_F9:  switchSceneManagerScene(8); break;
        case OF_KEY_F10: switchSceneManagerScene(9); break;
        case OF_KEY_F11: switchSceneManagerScene(10); break;
        case OF_KEY_F12: switchSceneManagerScene(11); break;
        case OF_KEY_BACKSPACE:
            switchSceneManagerScene(-1);
            break;
            
        case 'o':
            sceneManager.setOverlap(!sceneManager.getOverlap());
            break;
    }
}

//--------------------------------------------------------------
// dot not use from OSC messages.
// this is used because user is pressing a scene key change
// and we want to override osc scene index
void ofApp::switchSceneManagerScene(const int & sceneIndex){
    Globals::oscSceneIndex = -1;
    if (sceneIndex == -1) {
        sceneManager.noScene();
    } else {
        sceneManager.gotoScene(sceneIndex);
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
