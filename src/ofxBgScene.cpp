#include "ofxBgScene.h"

//--------------------------------------------------------------
void ofxBgScene::setup(){
}

//--------------------------------------------------------------
void ofxBgScene::update(){
    
}

//--------------------------------------------------------------
void ofxBgScene::draw(){
    ofColor startColor = ofColor::orange;
    ofColor endColor = ofColor::orangeRed;
    
	ofColor cs; cs.setHsb(startColor.getHue() + (ofSignedNoise(ofGetElapsedTimef()*0.29 -0.896)) * 22
                          , startColor.getSaturation()
                          , startColor.getBrightness());
    ofColor ce; ce.setHsb(endColor.getHue() + (ofSignedNoise(ofGetElapsedTimef()*0.19 +7.12)) * 22
                          , endColor.getSaturation()
                          , endColor.getBrightness());
    ofBackgroundGradient(cs, ce);
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
        default:
            break;
    }
}