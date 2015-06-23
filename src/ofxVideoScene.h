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
#include "ofxAppUtils.h"

class ofxVideoScene : public ofxFadeScene
{
public:
	ofxVideoScene(string fileName, string prefix = "scene") : 
		ofxFadeScene(prefix + ": " + "VideoScene " + fileName )
			, fileName(fileName) {
			setSingleSetup(true); // call setup each time the scene is loaded
			setFade(1000, 1000); // 1 second fade in/out
		}

	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	ofVideoPlayer player;

	string fileName;

	bool horizontalFlip;


};

