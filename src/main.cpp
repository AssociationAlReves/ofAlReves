#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(PROJECTOR_RESOLUTION_X,PROJECTOR_RESOLUTION_Y,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunAppWithAppUtils(new ofApp());

}
