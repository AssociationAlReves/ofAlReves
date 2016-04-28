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
#define CITY_BLOCK_TOTAL_WIDTH 1000
#define CITY_BLOCK_SIZE 50
#define CITY_BLOCKS_ROWS 5
#define CITY_BLOCKS_COLS 20
#define CITY_BLOCK_MAXHEIGHT 1000;


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
	// Movement
	float curSpeed;
	float curDistance;
	float curDistanceOffset;
	float desiredSpeed;

	void setupTextures();
	void setupRoad();
	void updateRoad();
	void generateBlocks();
	void setupBlocks();
	void updateBlocks();

	// road
	vector<ofPlanePrimitive> roads;
	ofFbo fboRoad;
	ofTexture texRoad;

	// buildings (aka blocks)
	float blockProbability;
	vector<ofBoxPrimitive> blockMeshes;
	vector<int> blocksL;	
	vector<int> blocksR;	

	// gui params
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
	bool bGuiLoaded;


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

