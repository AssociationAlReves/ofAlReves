#include "ofxSquareScreen.h"


//--------------------------------------------------------------
void ofxSquareScreen::setup(){
	mode = 0;
	width = ofGetWidth();
}

//--------------------------------------------------------------
void ofxSquareScreen::update(){

}

//--------------------------------------------------------------
void ofxSquareScreen::draw(){

	ofClear(0);
	ofFill();
	ofSetColor(255);

	int w = ofGetWidth();
	int h = ofGetHeight();

	switch (mode)
	{
	case SC_MODE_BEGIN :
	case SC_MODE_CLOSE :
	case SC_MODE_THIN :
		width = tween.update(); break;
	default:
		break;
	}

	ofRect((w-width)/2, 0, width, h);
}


//--------------------------------------------------------------
void ofxSquareScreen::nextMode(){
	mode = (mode + 1) % 5;  

	switch (mode)
	{
	case SC_MODE_INIT : setup(); break;
	case SC_MODE_BEGIN : {
		tween.setParameters(easingcirc, ofxTween::easeInOut,  ofGetWidth(),  ofGetWidth() -500, 5000,0);
						 } break;
	case SC_MODE_CLOSE : {
		tween.setParameters(easingsine, ofxTween::easeInOut,  ofGetWidth() -500,  300, 10000, 0);
						 } break;
	case SC_MODE_THIN : {
		tween.setParameters(easingquad, ofxTween::easeInOut,  300,  0, 10000, 0);
						 } break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofxSquareScreen::keyPressed(int key){
	switch (key)
	{
	case ' ' : nextMode(); break;
	case 'r': setup(); break;
	default:
		break;
	}
}