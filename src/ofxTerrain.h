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

#define VASA_HILL_MAX_AMP 0.1
#define VASA_HILL_RADIUS 15


#include "ofMain.h"
#include "globals.h"
#include "ofxAppUtils.h"
#include "ofxTween.h"

class ofxTerrain : public ofxFadeScene {
    
public:
    
    ofxTerrain(string prefix = "scene") : ofxFadeScene(prefix + ": " + "Terrain"){
			setSingleSetup(true); // call setup each time the scene is loaded
			setFade(1000, 1000); // 1 second fade in/out
		}
    
	void setup();
    void setup(int width, int height, int resolution, float velocity = 10);
    void update();
    void draw();
	void keyPressed(int key);

    void addHole(int x, int y);
    void addHill(int x, int y, float radius);
    
    int mode; // 0: normal, 1: to plane, 2: stop plane, 3: plane moving
    
    int planeWidth;
    int planeHeight;

    int planeResolution;

    ofVboMesh mesh;
    
    double deltaX;
    float sumDeltaX;
    
    double planeVelocity;
    float planeZscale;
    float noiseScale;
    float noiseSeed;
    float noiseAmp;
    
    float noiseScale2;
    float noiseSeed2;
    float noiseAmp2;

    ofFloatColor defaultColor;
	bool bSmallCursor;
    
private:
    bool drawYLines;
    float genNoise(int x, int y);
    float genNoise2(int x, int y);
    ofFloatColor getColor(float a);
    ofxEasingCubic tweenEasing;
    
    void updateHoles();
    vector<ofVec2f> holes;
    vector<float> holesAmp;
    
    void updateHills();
    vector<ofVec2f> hills;
    vector<float> hillsAmp;
    vector<float> hillsRadius;
    
    
    double speedRate;
    void setupLineMesh(int sizeX, int sizeY, int segmentLength);
    void setupTriangleMesh(int sizeX, int sizeY, int segmentLength);
    void setupCubeMesh(int sizeX, int sizeY, int segmentLength);
    void meshAppendX(int segmentLength, int numSegments);
    
    vector<float> heightMap;
    ofVec2f meshSize;
    
    int indexFromXY(int x, int y, int totalHeight);
    
    };