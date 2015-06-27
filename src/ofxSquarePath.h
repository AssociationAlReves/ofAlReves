//
//  ofxSquarePath.h
//  ofAlReves
//
//  Created by Xavier Fischer on 26/06/2015.
//
//
#pragma once

#define VASA_SQUARE_PADDING_FACTOR 0.25
#define VASA_SQUARE_DECAY_MIN 0.97
#define VASA_SQUARE_DECAY_MAX 0.985
#define VASA_SQUARE_SPEED 75
#define VASA_SQUARE_STARTY 10

#define SQUAREPATH_MODE_INIT 0  // squares appears, driven by azertyuiop keys
#define SQUAREPATH_MODE_SHAKE 1 // squares shakes, driven by azertyuiop keys
#define SQUAREPATH_MODE_FILL 2	// squares appears, driven by azertyuiop keys

#include "ofMain.h"
#include "globals.h"
#include "ofxAppUtils.h"

class ofxSquarePath : public ofxFadeScene {
    
public:
  	
	ofxSquarePath(string prefix = "scene") : ofxFadeScene(prefix + ": " + "SquarePath"){
			setSingleSetup(false); // call setup each time the scene is loaded
			setFade(1000, 1000); // 1 second fade in/out

			mode = SQUAREPATH_MODE_INIT;
		}

	void setup();
    void update();
    void draw();
	void keyPressed(int key);
    
    void clearActors();
    void addActor(int x, int y);
    
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
    
    vector<bool> squarePresence;
    vector<ofVec2f> actors;
    
};