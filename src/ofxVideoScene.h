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
	ofxVideoScene(string fileName, string prefix = "scene", bool autoPlay = false, bool fillScreen = true, bool hFlip = false, bool loop = true) :
		ofxFadeScene(prefix + ": " + "VideoScene " + fileName )
		, fileName(fileName), autoPlay(autoPlay), fillScreen(fillScreen), horizontalFlip(hFlip), autoLoop(loop) {
			setSingleSetup(false); // call setup each time the scene is loaded
			setFade(0,0);
		}

	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	ofVideoPlayer player;

	string fileName;
	bool autoPlay;
    bool autoLoop;
	bool fillScreen;
	float aspectRatio;

	bool horizontalFlip;


};

