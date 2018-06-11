//
//  ofxVisualPopupManager.h
//  ofAlReves
//
//  Created by Xavier Fischer on 11/06/2018.
//

#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxEasing.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"
#include "ofxVisualPopup.h"

class ofxVisualPopupManager : public ofxFadeScene
{
public:
    ofxVisualPopupManager(string prefix = "scene")
    : ofxFadeScene(prefix + ": " + "VisualPopup"){
        setSingleSetup(false); // call setup each time the scene is loaded
        setFade(500,500);
    }
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mouseMoved(int x, int y );
    
    
private:
    
    vector<ofxVisualPopup> images;
    int imgIndex;
    
};

