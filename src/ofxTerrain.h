//
//  ofxTerrain.h
//  displacementMap
//
//  Created by Xavier Fischer on 11/02/2015.
//
//
#pragma once

#define VASA_HOLE_MAX_AMP 5
#define VASA_HOLE_RADIUS 3

#define VASA_HILL_MAX_AMP 0.2  
#define VASA_HILL_RADIUS 15.0

#define VASA_TERRAIN_NORMAL 0
#define VASA_TERRAIN_TOPLANE 1
#define VASA_TERRAIN_STOPPED 2
#define VASA_TERRAIN_PLANEMOVING 3

#define VASA_TERRAIN_MINHOLE_DISTANCE 25.0


#include "ofMain.h"
#include "globals.h"
#include "ofxAppUtils.h"
#include "ofxTween.h"

class ofxTerrain : public ofxScene {
    
public:
    
    ofxTerrain(string prefix = "scene") : ofxScene(prefix + ": " + "Terrain"){
			setSingleSetup(false); // call setup each time the scene is loaded
		}
    
	void setup();
    void setup(int width, int height, int resolution, float velocity = 10);
    void update();
    void draw();
	void keyPressed(int key);

	ofPoint lastHoleCoords;
    void addHole(int x, int y);
    void addHill(int x, int y, float radius);
    

private:
	/*	0: VASA_TERRAIN_NORMAL, 
		1: VASA_TERRAIN_TOPLANE
		2: VASA_TERRAIN_STOPPED
		3: VASA_TERRAIN_PLANEMOVING
	*/
    int mode; 

    int planeWidth;
    int planeHeight;

    int planeResolution;

    ofVboMesh mesh;
    
	// Terrain scrolling
    double deltaX;
    float sumDeltaX;    
    double planeVelocity;
    float planeZscale;
   
    ofFloatColor defaultColor;
	bool bSmallCursor;
    
    bool drawYLines;
    
    void addHill_internal(float x, float y, float radius);


	//____________________________________________
	// Noise (little bumps)
    float genNoise(int x, int y);
    float genNoise2(int x, int y);
	float noiseScale;
	float noiseSeed;
	float noiseAmp;

	float noiseScale2;
	float noiseSeed2;
	float noiseAmp2;

    ofFloatColor getColor(float a);
    ofxEasingCubic tweenEasing;
    
    void updateHoles();
    vector<ofVec2f> holes;
    vector<float> holesAmp;
    
    void updateHills();
    vector<ofVec2f> hills;
    vector<float> hillsAmp;
    vector<float> hillsRadius;

	ofVec2f lastCursor;
	void updateCursor(ofVec2f position, bool direction);
    
    
    double speedRate;
    void setupLineMesh(int sizeX, int sizeY, int segmentLength);
    void setupTriangleMesh(int sizeX, int sizeY, int segmentLength);
    void setupCubeMesh(int sizeX, int sizeY, int segmentLength);
    void meshAppendX(int segmentLength, int numSegments);
    
    vector<float> heightMap;
    ofVec2f meshSize;
    
    int indexFromXY(int x, int y, int totalHeight);
    
    };