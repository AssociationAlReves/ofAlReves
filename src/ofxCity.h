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
#include "ofBuilding.h"

#define CITY_SPEED_INCR 1.
#define CITY_NUM_ROAD_PLANES 100
#define CITY_NUM_ROAD_PLANES_FADEIN 40
#define CITY_BLOCK_TOTAL_WIDTH 1000
#define CITY_BLOCK_SIZE 100
#define CITY_BLOCK_MARGIN_FACTOR 0.4
#define CITY_BLOCKS_ROWS 3
#define CITY_BLOCKS_COLS 50
#define CITY_BLOCK_MAXHEIGHT 4000
#define CITY_BLOCK_PAVEMENT_SIZE 50


enum CITY_MODE { enCityIdle = 0, 
	enCityStart,	// road appearance, opacity
	enCityBuildings,
	enCityRotate,
};

#define CITY_SETTINGS_FILE  "city_settings.xml"

class ofxCity: public ofxScene
{
public:
	ofxCity(string prefix = "scene") : 
		mode(enCityStart), bGuiLoaded(false), ofxScene(prefix + ": " + "City") {
			setSingleSetup(true); // call setup each time the scene is loaded
	}

	void setup();
	void update();
	void draw();

	void keyPressed(int key);

	int mode;


private:
	// Movement
	//float curSpeed;
	float curDistance;
	float curDistanceOffset;
	float desiredSpeed;


	void setupTextures();
	void setupRoad();
	void updateRoad(bool createNewRow);
	void setupBlocks();
	void updateBlocks(bool createNewRow);

	void generateBlockSide(bool isLeftSide);

	// road
	vector<ofPlanePrimitive> roads;
	ofFbo fboRoad;
	ofTexture texRoad;

	// buildings (aka blocks)
	//float blockProbability;
	vector<ofBuilding> buildings;
	vector<int> blocksL;	
	vector<int> blocksR;

	// gui params
	float roadParamsHash;
	ofxPanel gui;	 
	ofParameter<bool> autoGenerateBuildings;
	ofParameter<bool> debugFbo; 
	ofParameter<bool> bTweenSpeed;
	ofImage img;
	ofParameter<bool> bWireframe;
	ofParameter<float> fov;
	ofParameter<float> rotationAngle;
	ofParameter<float> roadLineWidth;
	ofParameter<float> roadLineHeight;
	ofParameter<int> roadTexWidth;
	ofParameter<int> roadTexHeight;
	ofParameter<float> curSpeed;
	ofParameter<float> roadOpacity;
	ofParameterGroup roadParams;

	ofParameter<float> blockProbability;
	ofParameterGroup buildingParams;
	bool bShowGui;
	bool bGuiLoaded;


	// transitions
	ofxTween tween;
	ofxTween tweenRoadOpactity;	
	ofxTween tweenRotate;

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
	ofMaterial material;
	// lights params
	ofParameter<ofVec3f> dirLightOrientation;
	ofParameter<ofFloatColor> diffuseColor;
	ofParameter<ofFloatColor> specularColor;
	ofParameter<ofFloatColor> ambientColor;
	ofParameterGroup lightParams;
};

