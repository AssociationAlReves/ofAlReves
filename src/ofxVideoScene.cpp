#include "ofxVideoScene.h"



//--------------------------------------------------------------
void ofxVideoScene::setup(){
	player.loadMovie("Sepio Ink in Water.mov");
	player.play();
}


//--------------------------------------------------------------
void ofxVideoScene::update(){
	player.update();
}

//--------------------------------------------------------------
void ofxVideoScene::draw(){
	ofPushMatrix();

	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	ofScale (1,-1,1);
	ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
	ofBackground(255,255,255);
	ofSetColor(255);
	player.draw(0,0, ofGetWidth(), ofGetHeight());

	ofPopMatrix();
}



//--------------------------------------------------------------
void ofxVideoScene::keyPressed(int key){
	switch (key)
	{
	case ' ' : player.play();
	case 'r': setup(); break;
	default:
		break;
	}
}