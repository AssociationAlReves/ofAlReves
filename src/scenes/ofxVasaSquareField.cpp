//
//  ofxVasaSquareField.cpp
//  ofVasarely
//
//  Created by Xavier Fischer on 05/05/2015.
//
//

#include "ofxVasaSquareField.h"

//--------------------------------------------------------------
void ofxVasaSquareField::setup(){

	switch (mode) {
	case VASA_SQUARE_MODE_FULL:
		setup(ofGetWidth(), ofGetHeight(), 50 /* 50 */);
		break;
	case VASA_SQUARE_MODE_ONE:
		setup(ofGetHeight()-100,ofGetHeight()-100,ofGetHeight()-100);
		break;
	case VASA_SQUARE_MODE_LINE:
		break;
	}

}

//--------------------------------------------------------------
void ofxVasaSquareField::setup(int width, int height, int squareSize){

	lastActorX = 0;
	lastActoxY = 0;
	//ary[i*sizeY+j]
	sizeX = width/squareSize+1;
	sizeY = height/squareSize+1;
	rotSpeed.reserve(sizeX*sizeY);
	rotDecay.reserve(sizeX*sizeY);
	for (int j = 0; j<sizeY;j++)
		for (int i = 0; i<sizeX;i++) {
			float speed = ofRandom(1, VASA_SQUARE_SPEED);
			rotMaxSpeed.push_back(speed);
			rotSpeed.push_back(speed);
			rotDecay.push_back(ofRandom(VASA_SQUARE_DECAY_MIN, VASA_SQUARE_DECAY_MAX));
		}

		this->squareTotalSize = squareSize;
		this->padding = squareSize*VASA_SQUARE_PADDING_FACTOR;
		this->squareSize = squareSize-padding;

}

//--------------------------------------------------------------
void ofxVasaSquareField::update(){

	clearActors();
	addActor(mouseX, mouseY);

	for (int i = 0; i<sizeX;i++)
		for (int j = 0; j<sizeY;j++)
		{
			int idx = i*sizeY+j;
			rotSpeed[idx] *= rotDecay[idx];

		}
}

//--------------------------------------------------------------
void ofxVasaSquareField::draw(){

	ofClear(ofColor::black);

	ofFill();
	ofSetColor(ofColor::white);

	ofPushMatrix();
	if (mode == VASA_SQUARE_MODE_ONE){
		ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
		int rad = ofGetHeight() - 100;
		ofRect(-rad/2,-rad/2, rad,rad);
	} else if (mode == VASA_SQUARE_MODE_FULL) {
		ofTranslate(squareSize,squareSize);
		for (int i = 0; i<sizeX;i++)
			for (int j = 0; j<sizeY;j++)
			{
				ofPushMatrix();
				ofTranslate(i*squareTotalSize-squareSize*(1.0*VASA_SQUARE_PADDING_FACTOR), j*squareTotalSize-squareSize*(1.0*VASA_SQUARE_PADDING_FACTOR));

				//            ofSetColor(255);
				//            ofPushMatrix();
				//            ofScale(-1,-1);
				//            ofDrawBitmapString(ofToString((int)rotSpeed[i*sizeY+j]), ofPoint(0,0));
				//            ofPopMatrix();
				//            ofSetColor(0);

				ofRotateZ(rotSpeed[i*sizeY+j] * ofGetElapsedTimef());
				ofRect(-squareSize/2,-squareSize/2, squareSize,squareSize);
				ofPopMatrix();

			}
	} else if (mode == VASA_SQUARE_MODE_LINE) {
	}

	ofPopMatrix();

	ofSetColor(ofColor::black);

}

//--------------------------------------------------------------
void ofxVasaSquareField::nextMode() {
	mode = (mode + 1) % 3;

	setup();
}

//--------------------------------------------------------------
void ofxVasaSquareField::addActor(int x, int y){
	x /= squareSize;
	y /= squareSize;

	if (x == lastActorX && y == lastActoxY) {
		return;
	}
	lastActorX = x;
	lastActoxY = y;
	//actors.push_back(ofVec2f(x,y));
	int radius = 1;
	for (int j = ofClamp(y-radius-1, 0, sizeY); j<ofClamp(y+radius, 0, sizeY);j++)
		for (int i = ofClamp(x-radius-1, 0, sizeX); i<ofClamp(x+radius, 0, sizeX);i++) {
			float speed = ofRandom(1, VASA_SQUARE_SPEED);
			rotMaxSpeed[i*sizeY+j] = speed;
			rotSpeed[i*sizeY+j] = speed;
			rotDecay[i*sizeY+j] = ofRandom(VASA_SQUARE_DECAY_MIN, VASA_SQUARE_DECAY_MAX);
		}
}

//--------------------------------------------------------------
void ofxVasaSquareField::clearActors(){
	actors.clear();
}

//--------------------------------------------------------------
void ofxVasaSquareField::hardReset(){
	for (int j = 0; j<sizeY;j++)
		for (int i = 0; i<sizeX;i++) {
			float speed = ofRandom(1, VASA_SQUARE_SPEED);
			rotMaxSpeed[i*sizeY+j] = speed;
			rotSpeed[i*sizeY+j] = speed;
			rotDecay[i*sizeY+j] = 0.8;
		}
}
//--------------------------------------------------------------
void ofxVasaSquareField::smoothReset(){
	for (int j = 0; j<sizeY;j++)
		for (int i = 0; i<sizeX;i++) {
			float speed = ofRandom(1, VASA_SQUARE_SPEED);
			rotMaxSpeed[i*sizeY+j] = speed;
			rotSpeed[i*sizeY+j] = speed;
			rotDecay[i*sizeY+j] = ofRandom(VASA_SQUARE_DECAY_MIN, VASA_SQUARE_DECAY_MAX);
		}
}


//--------------------------------------------------------------
void ofxVasaSquareField::keyPressed(int key){
	switch(key)
	{
	case 'c': hardReset();	break;
	case ' ': nextMode();	break;
	case 'w' : smoothReset(); break;
	case 'W' : hardReset(); break;
	}
}



