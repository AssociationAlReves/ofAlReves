//
//  ofxSquarePath.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 26/06/2015.
//
//

#include "ofxSquarePath.h"

//--------------------------------------------------------------
void ofxSquarePath::setup(){

	switch (mode) {
	case SQUAREPATH_MODE_INIT:
		setup(ofGetWidth(), ofGetHeight(), 50 /* 50 */);
		break;
	}

}

//--------------------------------------------------------------
void ofxSquarePath::setup(int width, int height, int squareSize){

	lastActorX = 0;
	lastActoxY = 0;
	//ary[i*sizeY+j]
	sizeX = width/squareSize+1;
	sizeY = height/squareSize+1;
	squarePresence.reserve(sizeX*sizeY);
	for (int j = 0; j<sizeY;j++)
		for (int i = 0; i<sizeX;i++) {
			int idx = i*sizeY+j;
			squarePresence[idx] = false;
		}

		this->squareTotalSize = squareSize;
		this->padding = squareSize*VASA_SQUARE_PADDING_FACTOR;
		this->squareSize = squareSize-padding;

}

//--------------------------------------------------------------
void ofxSquarePath::update(){
}

//--------------------------------------------------------------
void ofxSquarePath::draw(){

	ofClear(ofColor::black);

	ofFill();
	ofSetColor(ofColor::white);

	ofPushMatrix();
	switch (mode) {
	case SQUAREPATH_MODE_INIT: {
							   }
		break;
	}
	
	ofPopMatrix();

	ofSetColor(ofColor::black);

}

//--------------------------------------------------------------
void ofxSquarePath::nextMode() {
	mode = (mode + 1) % 3;

	setup();
}

//--------------------------------------------------------------
void ofxSquarePath::addActor(int x, int y){
	//x /= squareSize;
	//y /= squareSize;

	//if (x == lastActorX && y == lastActoxY) {
	//	return;
	//}
	//lastActorX = x;
	//lastActoxY = y;
	////actors.push_back(ofVec2f(x,y));
	//int radius = 1;
	//for (int j = ofClamp(y-radius-1, 0, sizeY); j<ofClamp(y+radius, 0, sizeY);j++)
	//	for (int i = ofClamp(x-radius-1, 0, sizeX); i<ofClamp(x+radius, 0, sizeX);i++) {
	//		float speed = ofRandom(1, VASA_SQUARE_SPEED);
	//		rotMaxSpeed[i*sizeY+j] = speed;
	//		rotSpeed[i*sizeY+j] = speed;
	//		rotDecay[i*sizeY+j] = ofRandom(VASA_SQUARE_DECAY_MIN, VASA_SQUARE_DECAY_MAX);
	//	}
}

//--------------------------------------------------------------
void ofxSquarePath::clearActors(){
	actors.clear();
}



//--------------------------------------------------------------
void ofxSquarePath::keyPressed(int key){
	switch(key)
	{
	case ' ': nextMode();	break;
	}
}



