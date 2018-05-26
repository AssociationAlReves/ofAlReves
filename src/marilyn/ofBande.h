//
//  ofBande.h
//  ofAlRevesTemp
//
//  Created by Xavier Fischer on 22/05/2018.
//

#pragma once
#include "ofMain.h"

enum bandeMode {
    BANDE_MODE_IDLE = 0,
    BANDE_MODE_GROW,
    BANDE_MODE_FOLD,
    BANDE_MODE_STACK
};

class ofBande{
    
public:
    ofBande();
    
    void setMode(bandeMode newMode);
    
    void reset();
    void update();
    void draw();
    
    ofColor color;
    ofPoint pos;
    ofPoint size;
    ofPoint sizeFactor; // x is top size, y is bottom size
    ofPoint initialPos;
    ofPoint vel;
    ofPoint frc;
    
    
    int screenHeight;
    int screenWidth;
    
    
    bandeMode mode;
    
    int currentMode;
    void nextMode(std::string reason);
    
    map<int, float> timeTriggers; // <state, time until next step>
    
    ofEasyCam cam;
    
   
};

