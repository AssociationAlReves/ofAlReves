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
ofxVisualPopup::ofxVisualPopup(string fileName, bool isVideo, float volume01, bool overlaps) :
    _isRunning(false), _isEnded(false), bSetupMode(false), _overlaps(overlaps), _isVideo(isVideo)
{
    ofSetVerticalSync(true);
    
    _settingsXmlFileName = fileName + "_settings.xml";
    
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
    gui.add(fillParam.set("fill",false));
    gui.add(preserveAspectRatioParam.set("keep ratio", false));
    gui.add(rotParam.set("rotation", 0, -180, 180));
    gui.add(color1.set("bg color1", ofColor(255)));
    gui.add(color2.set("bg color2", ofColor(255)));
    
    cout << "file " << fileName << " loaded. Size : " << _nativeSize.x << " / "
    << _nativeSize.y << endl;
    
    loadSettings();
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
}

//--------------------------------------------------------------
void ofxVisualPopup::draw(){
    //ofBackgroundGradient(color1, color2);
    
    ofEnableAlphaBlending();
    ofSetColor(255);
    ofPushMatrix();
    if (_isRunning || (_overlaps && _isEnded)) {
        
        // Check if filling OR custom rotation and size
        if (fillParam || preserveAspectRatioParam){
            //------------------
            // FILL
            if (fillParam) {
                if (_isVideo) {
                    _movie.draw(0, 0, Globals::screenWidth, Globals::screenHeight);
                } else {
                    _image.draw(0,0, Globals::screenWidth, Globals::screenHeight);
                }
            }
            else {
                float pw = 0;
                float ph = 0;
                float xCenterOrigin = 0;
                float yCenterOrigin = 0;
                if (_nativeSize.x / _nativeSize.y > Globals::screenWidth / Globals::screenHeight) {
                    pw = Globals::screenWidth;
                    ph = _nativeSize.y * pw / _nativeSize.x;
                    xCenterOrigin = 0;
                    yCenterOrigin = -(ph - Globals::screenHeight) / 2;
                }
                else {
                    pw = _nativeSize.x * Globals::screenHeight / _nativeSize.y;
                    ph = Globals::screenHeight;
                    xCenterOrigin = -(pw - Globals::screenWidth) / 2;
                    yCenterOrigin = 0;
                }
                ofTranslate(xCenterOrigin, yCenterOrigin);
                if (_isVideo) {
                    _movie.draw(0, 0, pw, ph);
                } else {
                    _image.draw(0,0,  pw, ph);
                }
                //player.draw(0, 0, pw, ph);
            }
            
            
        } else {
            ofPushMatrix();
            
            ofTranslate(posParam->x * Globals::screenWidth, posParam->y * Globals:: screenHeight);
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
        }
      
        
        
    }
    ofPopMatrix();
    
  
}

//--------------------------------------------------------------
void ofxVisualPopup::drawGui(){
    if (bSetupMode){
        gui.draw();
    }
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
        posParam.set(ofPoint(x / (float)Globals::screenWidth, ((float)Globals::screenHeight-y)  / (float)Globals::screenHeight));
    }
}
