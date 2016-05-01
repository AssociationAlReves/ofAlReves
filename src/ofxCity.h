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
#define CITY_NUM_ROAD_PLANES 10//100
#define CITY_BLOCK_TOTAL_WIDTH 1000
#define CITY_BLOCK_SIZE 100
#define CITY_BLOCK_MARGIN_FACTOR 0.2
#define CITY_BLOCKS_ROWS 3
#define CITY_BLOCKS_COLS 10
#define CITY_BLOCK_MAXHEIGHT 1000.


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
	//float curSpeed;
	float curDistance;
	float curDistanceOffset;
	float desiredSpeed;

	void setupTextures();
	void setupRoad();
	void updateRoad();
	void setupBlocks();
	void updateBlocks();
	
	void updateBlockSide(bool isLeftSide);

	// road
	vector<ofPlanePrimitive> roads;
	ofFbo fboRoad;
	ofTexture texRoad;

	// buildings (aka blocks)
	//float blockProbability;
	vector<ofBoxPrimitive> buildings;
	vector<int> blocksL;	
	vector<int> blocksR;

	// gui params
	float roadParamsHash;
	ofxPanel gui;	 
	ofParameter<bool> autoGenerateBuildings;
	ofParameter<bool> debugFbo; 
		ofImage img;
	ofParameter<bool> bWireframe;
	ofParameter<float> fov;
	ofParameter<float> roadWidth;
	ofParameter<float> roadHeight;	
	ofParameter<float> roadLineWidth;
	ofParameter<float> roadLineHeight;
	ofParameter<int> roadTexWidth;
	ofParameter<int> roadTexHeight;
	ofParameter<float> curSpeed;
	ofParameterGroup roadParams;

	ofParameter<float> blockProbability;
	ofParameterGroup buildingParams;
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

	// lights
	ofLight directionalLight;
	// lights params
	ofParameter<ofVec3f> dirLightOrientation;
	ofParameter<ofFloatColor> diffuseColor;
	ofParameter<ofFloatColor> specularColor;
	ofParameter<ofFloatColor> ambientColor;
	ofParameterGroup lightParams;
};

