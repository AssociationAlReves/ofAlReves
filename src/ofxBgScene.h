//
//  ofxBgScene.h
//  ofAlReves
//	Inspired from Camera Ribbon example
//
//  Created by Xavier Fischer on 01/07/2015.
//
//
#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxTween.h"
#include "ofxAppUtils.h"

class ofxBgScene : public ofxFadeScene
{
public:
    ofxBgScene(string prefix = "scene")
    : ofxFadeScene(prefix + ": " + "BgScene"){
        setSingleSetup(false); // jjjjjmjmMmjj setup each time the scene is loaded
        setFade(1000,1000);
    }
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mouseMoved(int x, int y );
    
    void clear();
    
private:
    
    //this holds all of our points
    vector<ofVec3f> points;
    //this keeps track of the center of all the points
    ofVec3f center;
    
    float maxZ;
    
    bool endLine;
    
    float sWidth;
    float sHeight;
    
    bool bBlue;
    bool bBackground;
    
    // transitions
    ofxTween tween;
    
    ofxEasingBack 	easingback;
    ofxEasingBounce 	easingbounce;
    ofxEasingCirc 	easingcirc;
    ofxEasingSine	easingsine;
    ofxEasingCubic 	easingcubic;
    ofxEasingElastic easingelastic;
    ofxEasingExpo 	easingexpo;
    ofxEasingLinear 	easinglinear;
    ofxEasingQuad 	easingquad;
    ofxEasingQuart 	easingquart;
    ofxEasingQuint 	easingquint;
};

