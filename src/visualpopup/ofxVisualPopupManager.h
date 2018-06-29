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

enum popupManagerScenario{
    COMIC_STRIP = 0,
    MOULINEX,
    SUPERMARKETLADY,
	BONBONS
};

enum popupManagerTransition{
    NONE = 0,
    POPUP
};

class ofxVisualPopupManager : public ofxFadeScene
{
public:
    ofxVisualPopupManager(string prefix = "scene", popupManagerScenario scenarioIndex = COMIC_STRIP, popupManagerTransition fadeMode = NONE)
    :  ofxFadeScene(prefix + ": " + "VisualPopup"){
        setSingleSetup(false); // call setup each time the scene is loaded
        setFade(0,0);
        _scenarioIndex = scenarioIndex;
    }
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mouseMoved(int x, int y );
    
    
private:
    
    int _scenarioIndex;
    vector<ofxVisualPopup*> images;
    int imgIndex;
    
    vector<ofImage> imgCol;
    vector<ofVec3f> imgPos;
    vector<int> imgColIndex;
    vector<float> imgSize;
    
};

