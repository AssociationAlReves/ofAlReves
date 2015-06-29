//
//  ofxVasaSquareField.h
//  ofVasarely
//
//  Created by Xavier Fischer on 05/05/2015.
//
//
#pragma once

#define VASA_SQUARE_PADDING_FACTOR 0.25
#define VASA_SQUARE_DECAY_MIN 0.97
#define VASA_SQUARE_DECAY_MAX 0.985
#define VASA_SQUARE_SPEED 75
#define VASA_SQUARE_MODE_FULL 0
#define VASA_SQUARE_MODE_ONE 1


#include "ofMain.h"
#include "globals.h"
#include "ofxAppUtils.h"

class ofxVasaSquareField : public ofxScene {
    
public:
  	
	ofxVasaSquareField(string prefix = "scene", int startMode = VASA_SQUARE_MODE_FULL) : ofxScene(prefix + ": " + "SquareField"){
			setSingleSetup(false); // call setup each time the scene is loaded

			mode = VASA_SQUARE_MODE_FULL;
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
    
private:
    
    void setup(int width, int height, int squareSize);
    
    int         squareSize;
    int         squareTotalSize;
    int         padding;
    
    int sizeX;
    int sizeY;
	int mode;
    
    int lastActorX;
    int lastActoxY;
    
    vector<float> rotMaxSpeed;
    vector<float> rotSpeed;
    vector<float> rotDecay;
    vector<ofVec2f> actors;
    
};