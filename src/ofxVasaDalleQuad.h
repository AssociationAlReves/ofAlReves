//
//  ofxVasaDalleQuad.h
//  ofVasarely
//
//  Created by Nicole & Xavier on 17/02/2015.
//
//
#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxAppUtils.h"
#include "ofxBingQuad.h"
#include "ofxVasaDalle.h"

class ofxVasaDalleQuad : public ofxFadeScene {
    
public:
    
	ofxVasaDalleQuad(bool blackAndWhite, string prefix = "scene") 
		: blackAndWhite(blackAndWhite), ofxFadeScene(prefix + ": " + "Vasa dalles"){
			setSingleSetup(false); // call setup each time the scene is loaded
			setFade(5000,5000);
		}
	void setup();
    void setup(int width, int height, int sizeOfTiles = 256, int maxDepthLevel = 3);
    void update();
    void draw();
	void keyPressed(int key);
    
    void clearActors();
    void addActor(int x, int y, ofVec2f velocity);
    
    void clear();
    
    void createDalle(int x, int y, int depth, int shapeType);
    void createDalle(int x, int y, int depth);
    
    bool enabled;
    bool dynamicPathEnabled;
	bool bLights;
    bool autoDalle3;
	bool infiniteLifeTime;
	
private:
    
	bool blackAndWhite;
    int tileSize;
    void createDalleFromQuadKey(string key, int shapeType);
    
    vector<ofVec2f> actors;
    vector<ofVec2f> velocities;
    
    map<string,ofxVasaDalle> dalles;
    
    int maxDepth;
    int w,h;
    
    ofxBingQuad bing;

	ofLight light;
    ofMaterial material;
    
};