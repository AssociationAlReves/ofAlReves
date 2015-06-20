//
//  ofxTerrain.h
//  displacementMap
//
//  Created by Xavier Fischer on 11/02/2015.
//
//
#pragma once

#define VASA_DALLE_SHAPE_HEIGHT_RATIO 0.2
#define VASA_DALLE_SHAPE_UNDEFINED -1
#define VASA_DALLE_SHAPE_CIRCLE 0
#define VASA_DALLE_SHAPE_SQUARE 1

#include "ofMain.h"

class ofxVasaDalle {
    
public:
    
    ofxVasaDalle();
    
    void setup(int dalleSize);
    void update();
    void draw();
    
    int x,y;
    
    int size;
    float seed;
    
    float alpha; // when 0, dalle is removed
    
    // 0: circle, 1: square, 2:triangle
    int shapeType;
    
private:
    
    float createdTime;
    vector<ofColor> palette;
    
    ofFloatColor squareColor;
    ofFloatColor shapeColor;
    
    ofBoxPrimitive box;
    
    ofMaterial material;
    
    
    float midSize;
    float shapeSize;
    
    float boxHeight;
    float shapeHeight;
};