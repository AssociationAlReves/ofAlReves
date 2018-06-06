//
//  ofxRibbonPart.h
//  ofAlReves
//
//  Created by Xavier Fischer on 06/06/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxEasing.h"


class ofxRibbonPart{
    
public:
    ofxRibbonPart();
    
    
    void addPoint(int x, int y);
    
    void reset();
    void update();
    void draw();
    void clear();
    
    bool mouseDriven;
    
    
    float speedParam;
    float rangeMinParam;
    float rangeMaxParam;
    
    
    //this holds all of our points
    vector<ofVec3f> points;
    //this keeps track of the center of all the points
    ofVec3f center;
    
    float maxZ;
    
    bool endLine;
    
    float sWidth;
    float sHeight;
    
    float tweenInFrom, tweenInTo;
    
};
