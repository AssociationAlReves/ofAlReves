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
#include "ofxTween.h"
#include "ofxAppUtils.h"

#define SC_MODE_INIT 0
#define SC_MODE_BEGIN 1
#define SC_MODE_LINE 2
#define SC_MODE_BEGIN_REOPEN 3
#define SC_MODE_CLOSE 4
#define SC_MODE_VOPEN 5

class ofxSquareScreen : public ofxScene
{
public:
	ofxSquareScreen(string prefix = "scene", bool openFromBottom = false) 
		: openFromBottom(openFromBottom), ofxScene(prefix + ": " + "Square screen"){
			setSingleSetup(false); // call setup each time the scene is loaded
		}

	void setup();
	void update();
	void draw();

	void nextMode();

	void keyPressed(int key);

private:
	int mode;
	
	
	bool openFromBottom;

	float sWidth;
	float sHeight;

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

