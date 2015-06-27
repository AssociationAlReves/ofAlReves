#include "ofxVideoScene.h"



//--------------------------------------------------------------
void ofxVideoScene::setup(){
	tweenEnabled = false;
	player.loadMovie(fileName);
	player.play();
	aspectRatio = player.getWidth()/player.getHeight();
	if (autoPlay == false) {
		player.stop();
	}
}


//--------------------------------------------------------------
void ofxVideoScene::update(){
	player.update();
}

//--------------------------------------------------------------
void ofxVideoScene::draw(){
    
    ofEnableAlphaBlending();
	ofPushMatrix();

	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	if (horizontalFlip == false) {
		ofScale (1,-1,1);
	}
	ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
	ofBackground(255,255,255, 255);
	if (tweenEnabled) {
		ofSetColor(255, 255, 255, tween.update());
	} else {
		ofSetColor(255, 255, 255, 255);
	}
	float pw = ofGetHeight() * aspectRatio;
	float ph = ofGetHeight();
	float xCenterOrigin = -(pw-ofGetWidth())/2;
	ofTranslate(xCenterOrigin, 0);
	player.draw(0, 0, pw, ph);
	if (boxed) {
		ofSetColor(0,0,0,255);
		ofSetLineWidth(5);
		ofLine(0,0,pw,0);
		ofLine(pw,0,pw,ph);
		ofLine(pw,ph,0,ph);
		ofLine(0,ph,0,0);
	}


	ofPopMatrix();
    ofDisableAlphaBlending();
}



//--------------------------------------------------------------
void ofxVideoScene::keyPressed(int key){
	switch (key)
	{
	case ' ' : {
				if (player.isPlaying()) {
					tweenEnabled = true;
					tween.setParameters(easinglinear, ofxTween::easeInOut, 255, 0, 500, 0);
				} else { 
					player.play(); 
				}
			   }break;
	case 'r': setup(); break;
	default:
		break;
	}
}