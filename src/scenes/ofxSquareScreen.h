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
#include "ofxAppUtils.h"

class ofxSquareScreen : public ofxFadeScene
{
public:
	ofxSquareScreen() : ofxFadeScene("SquareScreen"){
			setSingleSetup(true); // call setup each time the scene is loaded
			setFade(1000, 1000); // 1 second fade in/out
		}

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
};

