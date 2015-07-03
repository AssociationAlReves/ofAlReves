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
    
    ofColor cs; cs.setHsb(fmodf(startColor.getHue() + (ofNoise(ofGetElapsedTimef()*0.1+1.234)) * 22,255)
                          , startColor.getSaturation()
                          , startColor.getBrightness());
    ofColor ce; ce.setHsb(fmodf(endColor.getHue() + (ofNoise(ofGetElapsedTimef()*0.34 - 3.23)) * 22,255)
                          , endColor.getSaturation()
                          , endColor.getBrightness());
    ofBackgroundGradient(cs, ce);
    
    /*
    stringstream ss;
    ss << "cs : " + ofToString(cs.getHue()) << endl;
    ss << "ce : " + ofToString(ce.getHue());
    ofDrawBitmapStringHighlight(ss.str(), 10, 10);
     */
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