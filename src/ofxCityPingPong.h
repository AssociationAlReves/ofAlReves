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

#define PING_PONG_NUM_STEPS 25



class ofxCityPingPong : public ofxScene {

public:
	ofxCityPingPong(string prefix = "scene") : ofxScene(prefix + ": " + "PPCityPP")
	{
		bShowHelp = false;
		setSingleSetup(false); // call setup each time the scene is loaded

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

	vector<ofRectangle> rects;
	vector<bool> rectsVisible;
	void updateRects();
	int rectSize;
	int curMargin;

	int curRectVisIndex;


	bool guiInitialized;
};