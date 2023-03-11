//
//  ofxCityPingPong.h
//  ofAlReves
//
//  Created by Xavier Fischer on 08/10/2016.
//
//
#pragma once


#include "ofMain.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"

#define PING_PONG_NUM_STEPS 4

const string PING_PONG_SETTINGS_FILE = "pingpong_settings.xml";


class ofxCityPingPong : public ofxScene {

public:
	ofxCityPingPong(string prefix = "scene") : ofxScene(prefix + ": " + "PPCityPP")
	{
		bShowHelp = false;
		setSingleSetup(false); // call setup each time the scene is loaded
        guiInitialized = false;
		curRectVisIndex = 0;

		bSpaceMode = true;
	}

	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	bool bShowHelp;

	bool bSpaceMode;


	

private:

    /*------------------------*/
    // GUI
    bool bShowGui;
    bool guiInitialized;
    ofxPanel gui;
    ofParameter<int> ppSize;
    ofParameter<int> ppMargin;
    
    void initGui();
    void altRects();
    void normalRects();
	vector<ofRectangle> rects;
	vector<bool> rectsVisible;
	void updateRects();

	int curRectVisIndex;


};
