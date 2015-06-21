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
#include "..\ofxTween.h"
#include "ofxAppUtils.h"

#define SC_MODE_INIT 0
#define SC_MODE_BEGIN 1
#define SC_MODE_CLOSE 2
#define SC_MODE_THIN 3
#define SC_MODE_END 4

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

	void nextMode();

	int mode;

	void keyPressed(int key);

	int width;

	// transitions
	ofxTween tween;

	ofxEasingBack 	easingback;
	ofxEasingBounce 	easingbounce;
	ofxEasingCirc 	easingcirc;
	ofxEasingSine	easingsine;
	ofxEasingCubic 	easingcubic;
	ofxEasingElastic easingelastic;
	ofxEasingExpo 	easingexpo;
	ofxEasingLinear 	easinglinear;
	ofxEasingQuad 	easingquad;
	ofxEasingQuart 	easingquart;
	ofxEasingQuint 	easingquint;
};

