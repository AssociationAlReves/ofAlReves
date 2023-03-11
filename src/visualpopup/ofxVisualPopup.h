//
//  ofxVisualPopup.h
//  ofAlRevesPopArt
//
//  Created by Xavier Fischer on 30/05/2018.
//
// transform with setup mode. settings are stored per file..
#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"
#include "ofxEasing.h"
#include "globals.h"

class ofxVisualPopup {
    
public:
    ofxVisualPopup(string fileName, bool isVideo, float volume01 = 0, bool overlaps = false);
    void update();
    void draw();
    void drawGui();
    void play(); // reset video player
    void stop();
    void reset();
    void setRotation(float rot);
    void keyPressed(int key);
    void mouseMoved(int x, int y );
    
    
    void loadSettings();
    
private:
    
    
    bool bSetupMode;
    
    ofxPanel gui;
    ofParameter<ofPoint> posParam;
    ofParameter<float> scaleParam;
    ofParameter<float> rotParam;
    ofParameter<ofColor> color1;
    ofParameter<ofColor> color2;
    ofParameter<bool> fillParam;
    ofParameter<bool> preserveAspectRatioParam;
    
    //easing
    float startTime;
    float endTime;
    
    ofPoint _nativeSize;
    ofVideoPlayer _movie;
    ofImage _image;
    string _settingsXmlFileName;
    bool _isVideo;
    bool _overlaps; // sill drawn when stopped
    bool _isRunning;
    bool _isEnded;
    
};
