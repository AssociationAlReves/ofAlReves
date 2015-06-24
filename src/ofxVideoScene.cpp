#include "ofxVideoScene.h"



//--------------------------------------------------------------
void ofxVideoScene::setup(){
	player.loadMovie(fileName);
	player.play();
	aspectRatio = player.getWidth()/player.getHeight();
	player.stop();
}


//--------------------------------------------------------------
void ofxVideoScene::update(){
	player.update();
}

//--------------------------------------------------------------
void ofxVideoScene::draw(){
	ofPushMatrix();

	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	if (horizontalFlip == false) {
		ofScale (1,-1,1);
	}
	ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
	ofBackground(255,255,255);
	ofSetColor(255);
	float pw = ofGetHeight() * aspectRatio;
	float ph = ofGetHeight();
	float xCenterOrigin = -(pw-ofGetWidth())/2;
	ofTranslate(xCenterOrigin, 0);
	player.draw(0, 0, pw, ph);
	if (boxed) {
		ofSetColor(0);
		ofSetLineWidth(5);
		ofLine(0,0,pw,0);
		ofLine(pw,0,pw,ph);
		ofLine(pw,ph,0,ph);
		ofLine(0,ph,0,0);
	}


	ofPopMatrix();
}



//--------------------------------------------------------------
void ofxVideoScene::keyPressed(int key){
	switch (key)
	{
	case ' ' : {
				if (player.isPlaying()) {
					player.stop();
				} else { 
					player.play(); 
				}
			   }break;
	case 'r': setup(); break;
	default:
		break;
	}
}