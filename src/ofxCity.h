//
//  ofxCity.h
//  ofAlReves
//
//  Created by Xavier Fischer on 25/04/2016.
//
//
#pragma once


#include "ofMain.h"
#include "globals.h"
#include "ofxTween.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"

#define CITY_SPEED_INCR 1.
#define CITY_NUM_ROAD_PLANES 30

class ofxCity: public ofxScene
{
public:
	ofxCity(string prefix = "scene") : 
		bGuiLoaded(false), ofxScene(prefix + ": " + "City") {
			setSingleSetup(false); // call setup each time the scene is loaded
	}

	void setup();
	void update();
	void draw();

	void keyPressed(int key);

private:
	bool bGuiLoaded;
	float curSpeed;
	float curDistance;
	float curDistanceOffset;
	float desiredSpeed;

	void setupTextures();
	void setupRoad();
	void updateRoad();

	vector<ofPlanePrimitive> roads;
	ofFbo fboRoad;
	ofTexture texRoad;

	float roadParamsHash;
	ofxPanel gui;	 
	ofParameter<bool> bWireframe;
	ofParameter<float> roadWidth;
	ofParameter<float> roadHeight;	
	ofParameter<float> roadLineWidth;
	ofParameter<float> roadLineHeight;
	ofParameter<int> roadTexWidth;
	ofParameter<int> roadTexHeight;
	ofParameterGroup roadParams;
	bool bShowGui;


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

