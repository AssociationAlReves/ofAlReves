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
    ofxBgScene(bool animate = true, bool orange = true, bool black = false, string prefix = "scene")
    : bAnimate(animate),bBlack(black), ofxFadeScene(prefix + ": " + "BgScene"){
        setSingleSetup(false); // setup each time the scene is loaded
        setFade(0,0);
		bBlue = !orange;
		
    }
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mouseMoved(int x, int y );
    
    void clear();
    
private:
    
    bool bAnimate;
		bool bBlack;
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

