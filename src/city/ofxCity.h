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
#include "ofxEasing.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"
#include "ofBuilding.h"

#define CITY_SPEED_INCR 1 //0.2
#define CITY_NUM_ROAD_PLANES 100
#define CITY_NUM_ROAD_PLANES_FADEIN 40
#define CITY_BLOCK_TOTAL_WIDTH 1000
#define CITY_BLOCK_SIZE 40
#define CITY_BLOCK_MARGIN_FACTOR 0.4
#define CITY_BLOCKS_ROWS 3
#define CITY_BLOCKS_COLS 75
#define CITY_BLOCK_MAXHEIGHT 1000
#define CITY_BLOCK_PAVEMENT_SIZE 50

#define CITY_COLLAPSE_BOX_WIDTH 1000000
#define CITY_COLLAPSE_BOX_HEIGHT 1000000

using namespace glm;

enum CITY_MODE { enCityIdle = 0, 
	enCityStart = 1,	// road appearance, opacity
	enCityBuildings = 2,
	enCityCollapsing = 3,
	enCityCollapsed = 4,
    enCityAgain = 5,
    enCityExplosion = 6,
    enCityBlank = 7,
	enCityLine = 8
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
	void updateRoad(bool createNewRow);
	void setupExplosion();
	void updateExplosion();
	void drawExplosion();
	void setupBlocks();
	void updateBlocks(int createRowsCount = 1);
	void translateBlocksHeights();
	void captureCam();

	void generateBlockSide(bool isLeftSide, int nowRowForced = 0); // 0 means not forced

	void accelerate(int duration = 2000);
	void decelerate(int duration = 2000, bool stop = false);
	void setMode(int mode);

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
	ofParameter<float> translationCollapse;
	ofParameter<float> roadLineWidth;
	ofParameter<float> roadLineHeight;
	ofParameter<int> roadTexWidth;
	ofParameter<int> roadTexHeight;
	ofParameter<float> curSpeed;
	ofParameter<float> roadOpacity;
	ofParameter<vec3> camOrientation;
	ofParameter<vec3> camPosition;
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
	
	ofVec3f prevCamTranslate, curCamTranslate;
	ofVec3f prevCamRot, curCamRot;
	
	// lights
	ofLight directionalLight;	
	ofMaterial material;
	// lights params
	ofParameter<ofVec3f> dirLightOrientation;
	ofParameter<ofFloatColor> diffuseColor;
	ofParameter<ofFloatColor> specularColor;
	ofParameter<ofFloatColor> ambientColor;
	ofParameterGroup lightParams;
	
	// Transitions
	float speedFrom, speedTo;
	float speedStartTime, speedEndTime;
	float transFrom, transTo;
	float transInitTime, transEndTime;
	float explosionFrom, explosionTo;
	float explosionInitTime, explosionEndTime;
	float collapseFrom, collapseTo;
	float collapseInitTime, collapseEndTime;
	float boxWInitTime, boxWEndTime, boxWFrom, boxWTo;
	float boxHInitTime, boxHEndTime, boxHFrom, boxHTo;
	
};

