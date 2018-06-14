//
//  ofxVisualPopupManager.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 11/06/2018.
//

#include "ofxVisualPopupManager.h"
#include "ofApp.h"


//--------------------------------------------------------------
void ofxVisualPopupManager::setup(){
    images.clear();
    
    if (_scenarioIndex == 0) {
        ofxVisualPopup img4 = ofxVisualPopup("comic/vid/pow.mp4", true, 0, true);
        img4.loadSettings(); images.push_back(img4);
        ofxVisualPopup img0 = ofxVisualPopup("comic/img/pop.jpg", false, 0, true);
        img0.loadSettings(); images.push_back(img0);
        ofxVisualPopup img1 = ofxVisualPopup("comic/img/blop.jpg", false, 0, true);
        img1.loadSettings(); images.push_back(img1);
        ofxVisualPopup img2 = ofxVisualPopup("comic/img/wizz.jpg", false, 0, true);
        img2.loadSettings(); images.push_back(img2);
    }
    else if (_scenarioIndex == 1) {
    }
    
    imgIndex = -1;
    
}

//--------------------------------------------------------------
void ofxVisualPopupManager::update(){
    
#ifdef ALREVES_USE_OSC
    if (Globals::oscKeyPressed != 0){
        bool isSpaceKeyPressed = Globals::oscKeyPressed == ' ';
        keyPressed(Globals::oscKeyPressed);
        Globals::oscKeyPressed = 0; // reset for non repeating
        cout << "popup space key" << endl;
    }
#endif
    
    for (int i = 0; i < images.size(); i++) {
        images[i].update();
    }
    
}


//--------------------------------------------------------------
void ofxVisualPopupManager::draw(){
    
    ofClear(255);
    ofPushMatrix();
    ofTranslate(0,Globals::screenHeight);
    ofScale(1,-1,1);
   // ofTranslate(0,-Globals::screenHeight);
    
    for (int i = 0; i < images.size(); i++) {
        images[i].draw();
    }
    ofPopMatrix();
}



//--------------------------------------------------------------
void ofxVisualPopupManager::mouseMoved(int x, int y ){
    
     images[imgIndex].mouseMoved(x, y);
}



//--------------------------------------------------------------
void ofxVisualPopupManager::keyPressed(int key){
    if (key == ' '){
        if (imgIndex >= 0){
            images[imgIndex].stop();
        }
        imgIndex = (imgIndex + 1) % (images.size()+1);
        if (imgIndex == 0) {
            for (int i = 0; i < images.size(); i++) {
                images[i].reset();
            }
        } else if (imgIndex == images.size()){
            for (int i = 0; i < images.size(); i++) {
                images[i].reset();
            }
            imgIndex = -1;
        }
        cout << imgIndex << endl;
        
        if (imgIndex >= 0){
            images[imgIndex].play();
        }
    }
    if (key == 'r'){
        imgIndex = -1;
    }
    
    images[imgIndex].keyPressed(key);
}
