#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "scenes\globals.h"

// optional ofxGUI control panel for transformer,
// uncomment this if you're using ofxGui in this project
#define HAVE_OFX_GUI

#ifdef HAVE_OFX_GUI
#include "ofxGui.h"
#include "ofxTransformPanel.h"
#endif

class ofApp : public ofxApp { // <-- inherits ofxApp instead of ofBaseApp

public:

	ofApp()  {}

	void setupSceneManager();
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	void windowResized(int w, int h);

	// rendering transformer
	ofxTransformer transformer;

	// handles the scenes
	ofxSceneManager sceneManager;
	int lastScene;

	ofEasyCam cam;

#ifdef HAVE_OFX_GUI
	ofxTransformPanel panel;
#endif
};
