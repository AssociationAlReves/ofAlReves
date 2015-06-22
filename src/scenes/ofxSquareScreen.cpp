#include "ofxSquareScreen.h"


//--------------------------------------------------------------
void ofxSquareScreen::setup(){
	mode = 0;
	sWidth = ofGetWidth();
	sHeight = ofGetHeight();
}

//--------------------------------------------------------------
void ofxSquareScreen::update(){

}

//--------------------------------------------------------------
void ofxSquareScreen::draw(){

	int w = ofGetWidth();
	int h = ofGetHeight();

	switch (mode)
	{
	case SC_MODE_BEGIN :
	case SC_MODE_CLOSE :
		sWidth = tween.update(); break;
	case SC_MODE_VOPEN :
		sHeight = tween.update(); break;
	default:
		break;
	}


	ofPushMatrix();

	ofClear(0); 
	ofFill();
	ofSetColor(255); 

		switch (mode)
		{
		case SC_MODE_INIT:
		case SC_MODE_BEGIN:
		case SC_MODE_CLOSE: {
			ofTranslate((w - sWidth)/2, 0);
			ofRect(0, 0, sWidth, h);
							} break;
		case SC_MODE_VOPEN: {
			if (openFromBottom) {
				ofTranslate(0, (h-sHeight));
			} else {
				ofTranslate(0, 0);
			}
			ofRect(0, 0, w, sHeight);
							} break;
		default:
			break;
		}

	ofPopMatrix();


}


//--------------------------------------------------------------
void ofxSquareScreen::nextMode(){
	mode = (mode + 1) % 4;  

	switch (mode)
	{
	case SC_MODE_INIT : setup(); break;
	case SC_MODE_BEGIN : {
		tween.setParameters(easingcirc, ofxTween::easeInOut
			, ofGetWidth()
			, ofGetWidth()*0.375
			, 5000,0);
						 } break;
	case SC_MODE_CLOSE : {
		tween.setParameters(easingquad, ofxTween::easeInOut
			, ofGetWidth()*0.375
			, 0
			, 10000, 0);
						 } break;
	case SC_MODE_VOPEN : {
		sWidth = ofGetWidth();
		sHeight = 0;
		tween.setParameters(easingquad, ofxTween::easeInOut
			, 0
			, ofGetHeight()
			, 10000, 0);
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