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
        images.push_back(new ofxVisualPopup("comic/img/shebam.jpg", false, 0, true));
        images.push_back(new ofxVisualPopup("comic/img/pow.jpg", false, 0, true));
        images.push_back(new ofxVisualPopup("comic/img/blop.jpg", false, 0, true));
        images.push_back( new ofxVisualPopup("comic/img/wizz.jpg", false, 0, true));
        
        imgCol.clear();
        imgCol.push_back(ofImage("comic/buildings/b1.png"));
        imgCol.push_back(ofImage("comic/buildings/b2.png"));
        imgCol.push_back(ofImage("comic/buildings/b3.png"));
        imgCol.push_back(ofImage("comic/buildings/b4.png"));
        imgCol.push_back(ofImage("comic/buildings/b5.png"));
        imgCol.push_back(ofImage("comic/buildings/b6.png"));
        imgCol.push_back(ofImage("comic/buildings/b7.png"));
        imgCol.push_back(ofImage("comic/buildings/b8.png"));
        imgPos.clear();
        imgSize.clear();
        
    }
    else if (_scenarioIndex == MOULINEX) {
        images.clear();
        images.push_back(new ofxVisualPopup("vid/MixPub.mp4", true, 0, false));
    }
	else if (_scenarioIndex == SUPERMARKETLADY) {
		images.clear();
		images.push_back(new ofxVisualPopup("comic/img/supermarketlady.jpg", false, 0, false));
	} else if (_scenarioIndex == BONBONS) {
		images.clear();
		images.push_back(new ofxVisualPopup("vid/stripes1.mp4", true, 0, true));
		images.push_back(new ofxVisualPopup("vid/stripes2.mp4", true, 0, true));
		images.push_back(new ofxVisualPopup("vid/stripes3.mp4", true, 0, true));
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
    
    if (imgColIndex.size()>0){
        for(int i = 0; i < imgColIndex.size(); i++){
            ofVec3f pos = imgPos[i];
            float size = imgSize[i];
            imgCol[imgColIndex[i]].draw(pos.x,pos.y);
        }
    }
    
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
        imgPos.clear();
        imgColIndex.clear();
        imgSize.clear();
    }
    if (key == 'c'){
        imgPos.clear();
        imgColIndex.clear();
        imgSize.clear();
    }
    if (key == 'b'){
        int newimgIndex = (int)floor( ofRandom(0, 8));
        imgColIndex.push_back(newimgIndex);
        imgPos.push_back(ofVec3f(ofRandom(-200,Globals::screenWidth+100), 0,0));
        imgSize.push_back(ofRandom(1,1));
        
    }
    
    if (imgIndex >= 0){
        images[imgIndex]->keyPressed(key);
    }
}
