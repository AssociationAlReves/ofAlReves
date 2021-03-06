//
//  ofxVideoMirror.h
//  ofAlReves
//
//  Created by Xavier Fischer on 06/10/2016
//
//
#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxEasing.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"

enum MIRROR_MODE {
	enMirrorNormal = 0, // 50% screen left is projected to => screen / !screen (typical PhotoBooth effect)
	enMirror3Sides = 1, // 30% screen center is projected to => !screen / screen / !screen
	enMirrorMiddle = 2, // 50% screen center is projected to => screen / !screen
    enMirrorNSides = 3 // not working now
};

class ofxVideoMirror : public ofxFadeScene
{
public:
	ofxVideoMirror(string prefix = "scene", bool boxed = false) :
		ofxFadeScene(prefix + ": " + "VideoMirrorScene")
		{
		setSingleSetup(false); // call setup each time the scene is loaded
		setFade(0, 0);
		running = false;
		frameReady = false;
		guiInitialized = false;
		bShowGui = false;
	}
	bool running;
	bool frameReady;
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

private:

	void setMode(int mode);

	ofVideoGrabber 	vidGrabber; 
	
	//Pixels array for constructing output image
	ofPixels imagePixels;
	ofImage image; //Output image
    int deviceID;

	int camWidth;
	int camHeight;

	int curMode;
	float curMirrorFactor;
    int numSides;


	bool boxed;
	float aspectRatio;

	ofxPanel gui;
	ofxVec3Slider translation;
	bool guiInitialized;
	bool bShowGui;

};

