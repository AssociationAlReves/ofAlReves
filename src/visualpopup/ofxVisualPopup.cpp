//
//  ofxVisualPopup.cpp
//  ofAlRevesPopArt
//
//  Created by Xavier Fischer on 30/05/2018.
//

#include "ofxVisualPopup.h"


void ofxVisualPopup::setRotation(float rot){
    rotParam = rot;
}
//--------------------------------------------------------------
ofxVisualPopup::ofxVisualPopup(string fileName, bool isVideo, float volume01, bool overlaps){
    ofSetVerticalSync(true);
    
    _isRunning = false;
    _isEnded = false;
    bSetupMode = false;
    screenWidth = ofGetWidth();
    screenHeight = ofGetHeight();
    
    _settingsXmlFileName = fileName + "_settings.xml";
    _overlaps = overlaps;
    _isVideo = isVideo;
    
    if (_isVideo) {
        _movie.load(fileName);
        _movie.setVolume(volume01);
        ofPixels & pixels = _movie.getPixels();
        
        _nativeSize = ofPoint( pixels.getWidth(),pixels.getHeight());
        
    } else {
        _image.load(fileName);
        _nativeSize = ofPoint(_image.getWidth(), _image.getHeight());
    }
    
    gui.setup("VisualPopup", _settingsXmlFileName);
    gui.add(posParam.set("pos", ofPoint(0,0), ofPoint(0,0),ofPoint(ofGetWidth(),ofGetHeight())));
    gui.add(scaleParam.set("scale", 1, -10, 10));
    gui.add(rotParam.set("rotation", 0, -180, 180));
    gui.add(color1.set("bg color1", ofColor(255)));
    gui.add(color2.set("bg color2", ofColor(255)));
    
    cout << "file " << fileName << " loaded. Size : " << _nativeSize.x << " / "
    << _nativeSize.y << endl;
}

//--------------------------------------------------------------
void ofxVisualPopup::loadSettings(){
    cout << "Loading settings from " << _settingsXmlFileName << endl;
    gui.loadFromFile(_settingsXmlFileName);
}

//--------------------------------------------------------------
void ofxVisualPopup::reset(){
    _isRunning = false;
    _isEnded = false;
}

//--------------------------------------------------------------
void ofxVisualPopup::play(){
    loadSettings();
    if (_isVideo) {
        if ( _movie.isPlaying()){
            _movie.setFrame(0);
        } else {
            _movie.play();
        }
    } else {
       
    }
    
    _isRunning = true;
    
    // start easing
    startTime = ofGetElapsedTimef();
    endTime = startTime + 1;
    
}

//--------------------------------------------------------------
void ofxVisualPopup::stop(){
    if (_isVideo) {
        if ( _movie.isPlaying()){
            _movie.stop();
        }
    } else {
        
    }
    
    _isRunning = false;
    _isEnded = true;
}

//--------------------------------------------------------------
void ofxVisualPopup::update(){
    if (_isVideo) {
        _movie.update();
    } else {
        _image.update();
    }
    
    screenWidth = ofGetWidth();
    screenHeight = ofGetHeight();
}

//--------------------------------------------------------------
void ofxVisualPopup::draw(){
    //ofBackgroundGradient(color1, color2);
    
   
    ofPushMatrix();
    if (_isRunning || (_overlaps && _isEnded)) {
        ofPushMatrix();
        
        ofTranslate(posParam->x * screenWidth, posParam->y * screenHeight);
        ofScale(scaleParam, scaleParam);
        
        float now = ofGetElapsedTimef();
        if (now <= endTime) {
            
            // bounce or elastic are ok
            float easingValue = ofxeasing::map(now, startTime, endTime, 0, 1, &ofxeasing::bounce::easeOut);
            
            ofScale(easingValue, easingValue);
        }
        
        ofRotate(rotParam, 0, 0, 1);
        ofTranslate(-(_nativeSize.x)/2., -(_nativeSize.y) /2.);
        
        if (_isVideo) {
            _movie.draw(0,0);
        } else {
            _image.draw(0,0);
        }
        
        ofPopMatrix();
        
        if (bSetupMode){
            gui.draw();
        }
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofxVisualPopup::keyPressed(int key){
    if (key == 's'){
        bSetupMode = !bSetupMode;
    }
    if (key == 'S'){
        gui.saveToFile(_settingsXmlFileName);
    }
    if (key == 'L'){
        gui.loadFromFile(_settingsXmlFileName);
    }
    if (key == 'j'){
        rotParam-=1;
    }
    if (key == 'l'){
        rotParam+=1;
    }
    if (key == 'i'){
        scaleParam+=.1;
    }
    if (key == 'k'){
        scaleParam-=.1;
    }
}

//--------------------------------------------------------------
void ofxVisualPopup::mouseMoved(int x, int y ){
    if (bSetupMode) {
        posParam.set(ofPoint(x / (float)screenWidth, y / (float)screenHeight));
    }
}
