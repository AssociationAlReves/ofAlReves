#include "ofxBgScene.h"

//--------------------------------------------------------------
void ofxBgScene::setup(){
    bBackground = false;
}

//--------------------------------------------------------------
void ofxBgScene::update(){
    
}

//--------------------------------------------------------------
void ofxBgScene::draw(){
    ofColor startColor;
    ofColor endColor;

    if (bBlue){
        if (bBackground) {
           ofClear(ofColor::red);
        } else {
            ofClear(255);
        }
        startColor = ofColor(0, 200, 255);
        endColor = ofColor(0, 51, 255);
    } else {
        if (bBackground) {
            ofClear(ofColor(0, 0, 255));
        } else {
            ofClear(255);
        }
        startColor = ofColor::orangeRed;
        endColor = ofColor::orangeRed; //ofColor::orangeRed;
    }
    
    if (bAnimate) {
	ofColor cs; cs.setHsb(startColor.getHue() + (ofSignedNoise(ofGetElapsedTimef()*0.29 -0.896)) * 22
                          , startColor.getSaturation()
                          , startColor.getBrightness());
    ofColor ce; ce.setHsb(endColor.getHue() + (ofSignedNoise(ofGetElapsedTimef()*0.19 +7.12)) * 22
                          , endColor.getSaturation()
                          , endColor.getBrightness());
        
        ofBackgroundGradient(cs, ce);
    } else {
        ofBackgroundGradient(startColor, startColor);
    }
}

//--------------------------------------------------------------
void ofxBgScene::clear(){
}

//--------------------------------------------------------------
void ofxBgScene::mouseMoved(int x, int y ){
}



//--------------------------------------------------------------
void ofxBgScene::keyPressed(int key){
    switch (key)
    {
        case ' ' : clear(); break;
        case 'r': setup(); break;
        case 'c' : bBlue = !bBlue; break;
        case 'b' : bBackground = !bBackground; break;
        default:
            break;
    }
}