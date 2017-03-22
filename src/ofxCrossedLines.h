//
//  ofxSquareScreen.h
//  ofAlReves
//
//  Created by Xavier Fischer on 21/06/2015.
//
//
#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxEasing.h"
#include "ofxAppUtils.h"


class ofxCrossedLines: public ofxScene
{
public:
	ofxCrossedLines(bool starMode, string prefix = "scene") : 
		starMode(starMode), ofxScene(prefix + ": " + "Crossing lines") {
			setSingleSetup(false); // call setup each time the scene is loaded
		}

	void setup();
	void update();
	void draw();

	void keyPressed(int key);

private:

	void drawLine(int lineIndex, float interpolation = 1);
	int numLine;
	
	bool starMode;

	void nextMode();
	
	float start;
	float end;
	float initTime;
	float endTime;
};

