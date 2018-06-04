#pragma once

#include "ofMain.h"
#include "demoParticle.h"
#include "ofxGui.h"
#include "globals.h"
#include "ofxAppUtils.h"


#define POP_ART_PARTICLES_FILE  "popartparticles_settings.xml"

class ofParticles : public ofxFadeScene {
    
public:
    ofParticles(string prefix = "scene")
    :  ofxFadeScene(prefix + ": " + "Particles"){
        setSingleSetup(true); // call setup each time the scene is loaded
        setFade(5000,5000);
        bGuiLoaded = false;
    }
    void setup();
    void update();
    void draw();
    void resetParticles();
    
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
    
    particleMode currentMode;
    string currentModeStr;
    
    vector <demoParticle> p;
    vector <ofPoint> attractPoints;
    vector <ofPoint> attractPointsWithMovement;
    
    int numX;
    int numY;
    int gridSize;
    ofEasyCam cam;
    
    
    // gui params
    ofxPanel gui;
	ofParameter<int> gridSizeParam; // 10
    ofParameter<float> dragForceParam; //  0.9, 0.99
    ofParameter<float> forceParam; // 0.6
    ofParameter<float> returnForceParam; // 0.2
    ofParameter<float> distParam; // 150
    ofParameter<float> resetDistThresholdParam; // 10
    ofParameter<float> scaleParam;
    
    ofParameter<bool> autoGenerateBuildings;
    ofParameter<bool> debugFbo;
    ofParameter<bool> bTweenSpeed;
    
    ofParameterGroup colorGroup;
    ofParameter<ofColor> baseColorParam;
    ofParameter<ofColor> baseDestColorParam;
    ofParameter<ofColor> altColorParam;
    ofParameter<ofColor> altDestColorParam;
    ofParameter<float> colorVelocityParam;
    ofParameter<bool> useAltColor;
	ofParameter<bool> useAltBgColor;
    bool bShowGui;
    bool bGuiLoaded;
    
    // easing
    bool isExplosing;
    float initTime;
    float endTime;
};
