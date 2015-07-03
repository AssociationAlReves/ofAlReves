//
//  ofxVideoScene.h
//  ofAlReves
//
//  Created by Xavier Fischer on 22/06/2015.
//
//
#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxTween.h"
#include "ofxAppUtils.h"

class ofxVideoScene : public ofxFadeScene
{
public:
	ofxVideoScene(string fileName, string prefix = "scene", bool autoPlay = false, bool boxed = false, bool hFlip = false) : 
		ofxFadeScene(prefix + ": " + "VideoScene " + fileName )
		, fileName(fileName), autoPlay(autoPlay), boxed(boxed), horizontalFlip(hFlip) {
			setSingleSetup(false); // call setup each time the scene is loaded
			setFade(5000,5000);
		}

	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	ofVideoPlayer player;

	string fileName;
	bool autoPlay;
	bool boxed;
	float aspectRatio;

	bool horizontalFlip;

	bool tweenEnabled;
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

