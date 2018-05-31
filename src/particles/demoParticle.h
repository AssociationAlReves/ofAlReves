#pragma once
#include "ofMain.h"
#include "globals.h"

enum particleMode{
    PARTICLE_MODE_ATTRACT = 0,
    PARTICLE_MODE_REPEL,
    PARTICLE_MODE_NEAREST_POINTS,
    PARTICLE_MODE_NOISE,
    PARTICLE_MODE_IDLE
};

class demoParticle{
    
public:
    demoParticle();
    
    void setMode(particleMode newMode);
    void setAttractPoints( vector <ofPoint> * attract );
    
    void reset();
    void update();
    void draw();
    
    ofPoint pos;
    ofPoint initialPos;
    ofPoint vel;
    ofPoint frc;
    ofPoint frc2Init;
    
    ofColor color;
    ofColor destColor;
    float colorVelocity;
    
    float dragForce; //  0.9, 0.99
    float force; // 0.6
    float returnForce; // 0.2
    float dist; // 150
    float resetDistThreshold;
    
    int screenHeight;
    int screenWidth;
    
    float drag;
    float uniqueVal;
    float scale;
    
    particleMode mode;
    
    vector <ofPoint> * attractPoints;
};
