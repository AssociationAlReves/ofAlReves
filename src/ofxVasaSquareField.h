//
//  ofxVasaSquareField.h
//  ofVasarely
//
//  Created by Xavier Fischer on 05/05/2015.
//
//
#pragma once

#define VASA_SQUARE_PADDING_FACTOR 0.25
#define VASA_SQUARE_DECAY_MIN 0.97  //0.97
#define VASA_SQUARE_DECAY_MAX 0.985 // 0.98
#define VASA_SQUARE_SPEED 75 //75
#define VASA_SQUARE_MODE_FULL_RND_ROTATION 0
#define VASA_SQUARE_MODE_FULL_DIST_ROTATION 1
#define VASA_SQUARE_SIZE 50 // 30
#define VASA_SQUARE_ACTOR_RADIUS 3 // 1

#define VASA_SQUARE_SETTINGS_FILE  "squarefield_settings.xml"


#include "ofMain.h"
#include "globals.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"

class ofxVasaSquareField : public ofxFadeScene {
    
public:
  	
	ofxVasaSquareField(bool squareSpawn, string prefix = "scene", int startMode = VASA_SQUARE_MODE_FULL_RND_ROTATION)
		: squareSpawn(squareSpawn), mode(startMode), ofxFadeScene(prefix + ": " + "SquareField"){
			setSingleSetup(false); // call setup each time the scene is loaded
			setFade(0, 0);
			bGuiInitialized = false;
			bShowGui = true;
		}

	void setup();
    void update();
    void draw();
	void keyPressed(int key);
    
    void clearActors();
    void addActor(int x, int y);
    
    void hardReset();
    void smoothReset();

	void nextMode();
	int squareIndex;
	bool squareAutoComplete;

	bool bGuiInitialized;
	ofxPanel gui;
	bool bShowGui;
    ofParameterGroup forceParams;
	ofParameterGroup scaleParams;
	ofParameterGroup angleParams;

private:
    
    void setup(int width, int height, int squareSize);
    
    int         squareSize;
    int         squareTotalSize;
    int         padding;
    
    int sizeX;
    int sizeY;
	int squareCount;
	int mode;
		
    int lastActorX;
    int lastActoxY;
    
    // if true then squares spawns with space key
    // if false, all squares appears on startup
    bool squareSpawn;

	ofParameter<float> distRange;
	ofParameter<bool> distRangeAuto;
	ofParameter<float> distRangeSpeed;
	ofParameter<float> distRangeMIN;
	ofParameter<float> distRangeMAX;	

	ofParameter<float> scaleFactor;
	ofParameter<bool> scaleFactorAuto;
	ofParameter<float> scaleFactorSpeed;
	ofParameter<float> scaleFactorMIN;
	ofParameter<float> scaleFactorMAX;	

	ofParameter<float> devAngle;
	ofParameter<bool> devAngleAuto;
	ofParameter<float> devAngleSpeed;
	ofParameter<float> devAngleMIN;
	ofParameter<float> devAngleMAX;		
	
    vector<float> rotMaxSpeed;
    vector<float> rotSpeed;
    vector<float> rotDecay;
    vector<ofVec2f> actors;
	vector<ofVec2f> randIndexes;
    
};