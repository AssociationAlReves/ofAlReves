#include "ofxSquareScreen.h"


//--------------------------------------------------------------
void ofxSquareScreen::setup(){
	mode = SC_MODE_INIT;
	sWidth = ofGetWidth();
	sHeight = ofGetHeight();

	tween.setParameters(easingsine, ofxTween::easeInOut
			, sWidth
			, sWidth
			, 100,0);
}

//--------------------------------------------------------------
void ofxSquareScreen::nextMode(){
	mode = (mode + 1) % 6;  

	cout << "new mode : " << ofToString(mode) << endl;

	switch (mode)
	{
	case SC_MODE_INIT : setup(); break;
	case SC_MODE_BEGIN : {
		tween.setParameters(easingsine, ofxTween::easeInOut
			, ofGetWidth()
			, 0
			, 28000,0);
						 } break;
	case SC_MODE_LINE: {
		tween.setParameters(easingexpo, ofxTween::easeInOut
			, 0
			, PI
			, 500,0);
					   } break;
	case SC_MODE_BEGIN_REOPEN: {
		tween.setParameters(easingback, ofxTween::easeOut
			, 0
			, ofGetWidth()*0.3
			, 2000,0);
							   } break;
	case SC_MODE_CLOSE : {
		tween.setParameters(easingquad, ofxTween::easeInOut
			, ofGetWidth()*0.3
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
void ofxSquareScreen::update(){

}

//--------------------------------------------------------------
void ofxSquareScreen::draw(){

	int w = ofGetWidth();
	int h = ofGetHeight();

	int backColor = 0;

	// dimensions update
	switch (mode)
	{
	default :
		sWidth = tween.update(); break;
	case SC_MODE_VOPEN :
		sHeight = tween.update(); break;
	}


	// other updates (alpha, color, mode change)
	switch (mode)
	{
	case SC_MODE_BEGIN : if (tween.isCompleted()) nextMode(); break;
	case SC_MODE_LINE : // same as begin but color
		{
			backColor = (int)(abs(sin(2.f*sWidth))*255);
			if (tween.isCompleted()) nextMode();
		}
		break;
	default:
		break;
	}

	int foreColor = 255-backColor;
	ofPushMatrix();

	ofBackground(backColor);
	ofFill();
	ofSetColor(foreColor); 

	switch (mode)
	{
	case SC_MODE_LINE: {
		float lineWidth = (float)w * sin(sWidth);
		//cout << ofToString(lineWidth) << endl;
		ofTranslate((w - lineWidth)/2, (h/2-5));
		ofRect(0, 0, lineWidth, 5);
					   } break;
	case SC_MODE_VOPEN: {
		if (openFromBottom) {
			ofTranslate(0, (h-sHeight));
		} else {
			ofTranslate(0, 0);
		}
		ofRect(0, 0, w, sHeight);
						} break;
	default: {
		ofTranslate((w - sWidth)/2, 0);
		ofRect(0, 0, sWidth, h);
			 } 
			 break;
	}

	ofPopMatrix();


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