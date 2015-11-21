//
//  ofxVasaCubeMesh.h
//  ofAlReves
//
//  Created by Xavier Fischer on 16/07/2015.
//
//

#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxTween.h"
#include "ofxAppUtils.h"

class ofxVasaCubeMesh : public ofxFadeScene
{
public:
    ofxVasaCubeMesh(string prefix = "scene")
    : ofxFadeScene(prefix + ": " + "Vasa Cube Mesh"){
        setSingleSetup(false); // call setup each time the scene is loaded
        setFade(200,200);
    }
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mouseMoved(int x, int y );
    
    void clear();
    
private:
    
    ofVboMesh mesh;
    
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

