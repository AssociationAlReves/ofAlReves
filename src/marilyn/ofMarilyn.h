//
//  ofMarilyn.h
//  ofAlReves
//
//  Created by Xavier Fischer on 26/05/2018.
//

#pragma once

#include "ofMain.h"
#include "ofBande.h"
#include "ofxGui.h"
#include "ofxEasing.h"
#include "ofxPostGlitch.h"
#include "ofxAppUtils.h"

#define BANDE_SETTINGS_FILE  "bande_settings.xml"

#define MOV_BANDES_DEBUG
#define MOV_BANDES_DEF_NO_TIME 99999

#define MOV_state_Blank             0    // initial screen
#define MOV_state_AllDown  1    // 0:08 à 0:13 et 0:11
#define MOV_state_ToutRecule        2    // 0:13 à 0:13,1
#define MOV_state_ToutAvanceMid     3    // 0:13 à 0:28
#define MOV_state_DepartJardin  4    // 53.5
#define MOV_state_Bleu  5
#define MOV_state_Rose  6
#define MOV_state_Violet  7
#define MOV_state_Orange  8
#define MOV_state_Vert  9
///....
#define MOV_state_i  10 // i
#define MOV_state_ii  11 // ii
#define MOV_state_iii  12 // iii
#define MOV_state_ivacour  13 ///iva
#define MOV_state_ivbjardin  14 ///ivb
#define MOV_state_ivccour 15 /// ivc
#define MOV_state_ivdmilieu 16 /// ivd
#define MOV_state_vmelange 17 /// ivd
#define MOV_state_1col2 18
#define MOV_state_1col4 19
#define MOV_state_1col0 20
#define MOV_state_1col3 21
#define MOV_state_1col1 22
#define MOV_state_allCol 23
#define MOV_state_shuffle 24
#define MOV_state_glitch 25


#define EASING_LINEAR 0
#define EASING_SINE 1
#define EASING_EXPO 2
#define EASING_CUBIC 3
#define EASING_BACK 4
#define EASING_QUAD 5
#define EASING_LINEAR_EASEOUT 6
#define EASING_SINE_EASEOUT 7
#define EASING_EXPO_EASEOUT 8
#define EASING_CUBIC_EASEOUT 9
#define EASING_BACK_EASEOUT 10
#define EASING_QUAD_EASEOUT 11


class ofMarilyn : public ofxFadeScene{
    
public:
    ofMarilyn(string prefix = "scene")
    : ofxFadeScene(prefix + ": " + "Marilyn"){
        setSingleSetup(false); // setup each time the scene is loaded
        setFade(0,0);
        
    }
    void setup();
    void update();
    void draw();
    void resetBandes(bool forceFullSize);
    void setupTimeTriggers();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    int currentMode;
    
    vector <ofBande> p;
        
    ofEasyCam cam;
    
    
    // gui params
    ofxPanel gui;
    ofParameter<ofColor> color0Param;
    ofParameter<ofColor> color1Param;
    ofParameter<ofColor> color2Param;
    ofParameter<ofColor> color3Param;
    ofParameter<ofColor> color4Param;
    ofParameter<bool> blankStartParam;
    //    ofParameter<float> dragForceParam; //  0.9, 0.99
    //    ofParameter<float> forceParam; // 0.6
    //    ofParameter<float> returnForceParam; // 0.2
    //    ofParameter<float> distParam; // 150
    //    ofParameter<float> resetDistThresholdParam; // 10
    //
    //    ofParameter<bool> autoGenerateBuildings;
    //    ofParameter<bool> debugFbo;
    //    ofParameter<bool> bTweenSpeed;
    bool bShowGui;
    bool bGuiLoaded;
    
    
    // easing
    bool isExplosing;
    float initTime;
    float endTime;
    
    bool isRotating;
    bool isStopFadeOut;
    bool isMirrored;
    
    float currentAngle;
    
    // modes
    map<int, float> timeTriggers; // <state, time until next step>
    void nextMode(std::string reason);
    // transitions
    float tweenDuration;
    float noiseFrom, noiseTo;
    float noiseInitTime, noiseEndTime;
    float fadeInFrom, fadeInTo;
    float fadeinInitTime, fadeInEndTime;
    int noiseEasing;
    int fadeEasing;
    
    void custom_bandeFullScreen(const int bandeIndex);
    
    bool beforeDraw();
    
    void updateTweenA(int easing, float _from, float _to, float _duration = 3000 );
    float getTweenAValue();
    float getTweenBValue();
    void updateTweenB(int easing, float _from, float _to, float _duration = 3000 );
    
    /*Addon instance*/
    ofxPostGlitch    myGlitch;
    ofFbo            glitchFbo;
};
