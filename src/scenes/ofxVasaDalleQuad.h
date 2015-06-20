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
#include "quads\ofxBingQuad.h"
#include "quads\ofxVasaDalle.h"

class ofxVasaDalleQuad : public ofxFadeScene {
    
public:
    
	ofxVasaDalleQuad() : ofxFadeScene("Vasa dalles"){
			setSingleSetup(true); // call setup each time the scene is loaded
			setFade(1000, 1000); // 1 second fade in/out

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
private:
    
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