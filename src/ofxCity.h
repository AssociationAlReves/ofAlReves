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
#include "ofxCityPingPong.h"

#define CITY_SPEED_INCR 0.2
#define CITY_NUM_ROAD_PLANES 100
#define CITY_NUM_ROAD_PLANES_FADEIN 40
#define CITY_BLOCK_TOTAL_WIDTH 1000
#define CITY_BLOCK_SIZE 40
#define CITY_BLOCK_MARGIN_FACTOR 0.4
#define CITY_BLOCKS_ROWS 3
#define CITY_BLOCKS_COLS 75
#define CITY_BLOCK_MAXHEIGHT 1000
#define CITY_BLOCK_PAVEMENT_SIZE 50


enum CITY_MODE { enCityIdle = 0, 
	enCityStart,	// road appearance, opacity
	enCityBuildings,
	enCityCollapsing,
	enCityCollapsed,
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

	bool bUpdateParamsFromCode;


	void setupTextures();
	void setupRoad();
	void setupTerrain();
	void updateRoad(bool createNewRow);
	void setupBlocks();
	void updateBlocks(int createRowsCount = 1);
	void translateBlocksHeights();

	void generateBlockSide(bool isLeftSide, int nowRowForced = 0); // 0 means not forced
	void generateBlock_TheBigOne(); // huge building covering EVERYTHING, including you and the audience


	// road
	vector<ofPlanePrimitive> roads;
	ofVboMesh terrain;
	vector<float> heightMap;
	float genNoise2(const int x, const int y);
	int indexFromXY(const int x, const int y, const int totalHeight);
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
	ofParameter<float> translationCollapse;
	ofParameter<float> roadLineWidth;
	ofParameter<float> roadLineHeight;
	ofParameter<int> roadTexWidth;
	ofParameter<int> roadTexHeight;
	ofParameter<float> curSpeed;
	ofParameter<float> roadOpacity;
	ofParameter<ofVec3f> camOrientation;
	ofParameter<ofVec3f> camPosition;
	ofParameterGroup roadParams;

	ofParameter<int> terrainWidth;
	ofParameter<int> terrainHeight;
	ofParameter<int> segmentLength;
	ofParameter<bool> terrainDrawX;
	ofParameter<bool> terrainDrawY;
	ofParameter<float> terrainZScale;
	ofParameter<float> terrainNoiseScale;
	ofParameter<float> terrainNoiseSeed;
	ofParameter<float> terrainNoiseAmp;
	ofParameterGroup terrainParams;

	ofParameter<float> blockProbability;
	ofParameterGroup buildingParams;
	bool bShowGui;
	bool bGuiLoaded;
	
	// transitions
	ofxTween tween;
	ofxTween tweenRoadOpactity;	
	ofxTween tweenTranslate;
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

