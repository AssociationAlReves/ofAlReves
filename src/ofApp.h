#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "globals.h"
#include "ofxPostProcessing.h"
#include "ofxOcean.h"

class ofApp : public ofxApp {

	public:

		void setupSceneManager();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		
		// rendering transformer
		ofxTransformer transformer;

		// handles the scenes
		ofxSceneManager sceneManager;

		ofxOcean *oceanScene;

		ofEasyCam cam;

		ofxPostProcessing post;
		map<int, bool> postEnabledByScene;

		string IntToString(int i);

#ifdef HAVE_OFX_GUI
		ofxTransformPanel panel;
#endif
};
