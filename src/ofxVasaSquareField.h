//
//  ofxVasaSquareField.h
//  ofVasarely
//
//  Created by Xavier Fischer on 05/05/2015.
//
//
#pragma once

#define VASA_SQUARE_PADDING_FACTOR 0.25
#define VASA_SQUARE_DECAY_MIN 0.93  //0.97
#define VASA_SQUARE_DECAY_MAX 0.97 // 0.985
#define VASA_SQUARE_SPEED 75
#define VASA_SQUARE_MODE_FULL_RND_ROTATION 0
#define VASA_SQUARE_MODE_FULL_DIST_ROTATION 1

//#define VASA_SQUARE_DEBUG_SPIR


#include "ofMain.h"
#include "globals.h"
#include "ofxUI.h"
#include "ofxAppUtils.h"

class ofxVasaSquareField : public ofxScene {
    
public:
  	
	ofxVasaSquareField(string prefix = "scene", int startMode = VASA_SQUARE_MODE_FULL_RND_ROTATION) 
		: mode(startMode), ofxScene(prefix + ": " + "SquareField"){
			setSingleSetup(false); // call setup each time the scene is loaded
			guiInitDone = false;
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

	bool guiInitDone;
	ofxUISuperCanvas *gui;
    void guiEvent(ofxUIEventArgs &e);
    
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

	float distRange;	bool distRangeAuto;		float distRangeSpeed;		float distRangeMIN;		float distRangeMAX;	
	float scaleFactor;	bool scaleFactorAuto;	float scaleFactorSpeed; 	float scaleFactorMIN;	float scaleFactorMAX;	
	float devAngle;		bool devAngleAuto;		float devAngleSpeed; 		float devAngleMIN;		float devAngleMAX;		
	
    vector<float> rotMaxSpeed;
    vector<float> rotSpeed;
    vector<float> rotDecay;
    vector<ofVec2f> actors;
	vector<ofVec2f> randIndexes;
    
};