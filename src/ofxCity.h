//
//  ofxCity.h
//  ofAlReves
//
//  Created by Xavier Fischer on 25/04/2016.
//
//
#pragma once


#include "ofMain.h"
#include "globals.h"
#include "ofxTween.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"


class ofxCity: public ofxScene
{
public:
	ofxCity(string prefix = "scene") : 
		ofxScene(prefix + ": " + "City") {
			setSingleSetup(false); // call setup each time the scene is loaded
		}

	void setup();
	void update();
	void draw();

	void keyPressed(int key);

private:
	ofPlanePrimitive plane;
	ofFbo fboRoad;
	ofTexture texRoad;

	
     ofxPanel gui;	 
     ofParameter<bool> bWireframe;	 
     bool bShowGui;
};

