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
#include "ofxRibbonPart.h"

#define RIBBON_MODE_FREE 0
#define RIBBON_MODE_HLINE 1
#define RIBBON_MODE_CIRCLE 2
#define RIBBON_MODE_SNAKE 3
#define RIBBON_MODE_HLINE_BOTTOM 4
#define RIBBON_MODE_VLINES 5


#define RIBBON_SETTINGS_FILE  "ribbon_settings.xml"

class ofxRibbon : public ofxFadeScene
{
public:
	ofxRibbon(string prefix = "scene") 
		: ofxFadeScene(prefix + ": " + "Ribbon"){
			setSingleSetup(false); // call setup each time the scene is loaded
            setFade(0,0);
            bGuiLoaded = false;
            bShowGui = false;
            mode = 0;
        }

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void mouseMoved(int x, int y );

    void nextMode();

	void clear();
	
	void setCamera();

private:
	
    int mode;
    // gui params
    ofxPanel gui;
    
    ofParameter<float> speedParam; // 2
    
    ofParameter<float> rangeMinParam;
    ofParameter<float> rangeMaxParam;
    
    ofParameter<float> snakeSpeedParam;
    ofParameter<int> snakeStepsParam;
    bool bGuiLoaded;
    bool bShowGui;
    
	//this holds all of our points
    vector<ofxRibbonPart> parts;

    float sWidth;
    float sHeight;
    
    float now;
    float startTime;
    float endTime;
    float yOffset;

};

