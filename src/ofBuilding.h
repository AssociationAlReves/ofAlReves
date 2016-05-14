//
//  ofxTerrain.h
//  displacementMap
//
//  Created by Xavier Fischer on 11/02/2015.
//
//
#pragma once


#include "ofMain.h"

#define CITY_BOX_RESOLUTION 5

class ofBuilding {

public:

	ofBuilding(ofVec3f pos, float width, float height, float depth) :
		position(pos),width(width),height(height),depth(depth) {	

			box = ofBoxPrimitive(width, height, depth, CITY_BOX_RESOLUTION,CITY_BOX_RESOLUTION,CITY_BOX_RESOLUTION);
			box.setPosition(pos);
	}

	void setup();
	void update();
	void draw();
	void drawWireframe();

	int x,y;
	int size;

	float alpha; 

	ofVec3f position;
	
	ofBoxPrimitive box;

private:

	
	float width;
	float height;
	float depth;
};