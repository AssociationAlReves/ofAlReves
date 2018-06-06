//
//  ofxRibbon.h
//  ofAlReves
//	Inspired from Camera Ribbon example
//
//  Created by Xavier Fischer on 01/07/2015.
//
//
#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxEasing.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"


#define RIBBON_SETTINGS_FILE  "ribbon_settings.xml"

class ofxRibbon : public ofxFadeScene
{
public:
	ofxRibbon(string prefix = "scene") 
		: ofxFadeScene(prefix + ": " + "Ribbon"){
			setSingleSetup(false); // call setup each time the scene is loaded
            setFade(500,500);
            bGuiLoaded = false;
            bShowGui = false;
        }

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void mouseMoved(int x, int y );

	void clear();

private:
	
    // gui params
    ofxPanel gui;
    
    ofParameter<float> speedParam; // 2
    bool bGuiLoaded;
    bool bShowGui;
    
	//this holds all of our points
    vector<ofVec3f> points;
    //this keeps track of the center of all the points
    ofVec3f center;

	float maxZ;

	bool endLine;
	
	float sWidth;
	float sHeight;
	
	float tweenInFrom, tweenInTo;
};

