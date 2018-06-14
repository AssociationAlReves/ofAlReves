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
    
    
    if (_scenarioIndex == COMIC_STRIP) {
        images.clear();
        images.push_back(new ofxVisualPopup("comic/vid/pow.mp4", true, 0, true));
        images.push_back(new ofxVisualPopup("comic/img/pop.jpg", false, 0, true));
        images.push_back(new ofxVisualPopup("comic/img/blop.jpg", false, 0, true));
        images.push_back( new ofxVisualPopup("comic/img/wizz.jpg", false, 0, true));
    }
    else if (_scenarioIndex == MOULINEX) {
        images.clear();
        images.push_back(new ofxVisualPopup("comic/img/moulinex1.jpg", false, 0, true));
        images.push_back(new ofxVisualPopup("comic/img/moulinex2.jpg", false, 0, true));
        images.push_back(new ofxVisualPopup("comic/img/moulinex3.jpg", false, 0, true));
        images.push_back(new ofxVisualPopup("comic/img/moulinex4.jpg", false, 0, true));
        images.push_back(new ofxVisualPopup("comic/img/moulinex5.jpg", false, 0, true));
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
        images[i]->update();
    }
    
}


//--------------------------------------------------------------
void ofxVisualPopupManager::draw(){
    
    if (_scenarioIndex == COMIC_STRIP) {
        ofClear(255);
    } else if (_scenarioIndex == MOULINEX) {
        ofClear(0);
    } else {
        ofClear(0);
    }
    ofPushMatrix();
    ofTranslate(0,Globals::screenHeight);
    ofScale(1,-1,1);
    // ofTranslate(0,-Globals::screenHeight);
    
    if (_scenarioIndex == COMIC_STRIP) {
        for (int i = 0; i < images.size(); i++) {
            images[i]->draw();
        }
    } else {
        if (imgIndex >= 0) {
            images[imgIndex]->draw();
        }
    }
    ofPopMatrix();
    
    for (int i = 0; i < images.size(); i++) {
        images[i]->drawGui();
    }
}



//--------------------------------------------------------------
void ofxVisualPopupManager::mouseMoved(int x, int y ){
    
    if (imgIndex >= 0){
        images[imgIndex]->mouseMoved(x, y);
    }
}



//--------------------------------------------------------------
void ofxVisualPopupManager::keyPressed(int key){
    if (key == ' '){
        if (imgIndex >= 0){
            images[imgIndex]->stop();
        }
        imgIndex = (imgIndex + 1) % (images.size()+1);
        if (imgIndex == 0) {
            for (int i = 0; i < images.size(); i++) {
                images[i]->reset();
            }
        } else if (imgIndex == images.size()){
            if (_scenarioIndex == COMIC_STRIP) {
                for (int i = 0; i < images.size(); i++) {
                    images[i]->reset();
                }
                imgIndex = -1;
            } else
            {
                imgIndex = 0;
            }
        }
        cout << imgIndex << endl;
        
        if (imgIndex >= 0){
            images[imgIndex]->play();
        }
    }
    if (key == 'r'){
        imgIndex = -1;
    }
    
    if (imgIndex >= 0){
        images[imgIndex]->keyPressed(key);
    }
}
