#include "ofxSquareScreen.h"


//--------------------------------------------------------------
void ofxSquareScreen::setup(){
	mode = SC_MODE_INIT;
	sWidth = ofGetWidth();
	sHeight = ofGetHeight();
	
	easingInFrom = ofGetElapsedTimef();
	easingInTo = easingInFrom + 0.1f;
	easingOutFrom = sWidth;
	easingOutTo = sWidth;
	easingType = EASING_LINEAR;
}

//--------------------------------------------------------------
void ofxSquareScreen::nextMode(){
	mode = (mode + 1) % 6;  

	float now = ofGetElapsedTimef();
	
	cout << "new mode : " << ofToString(mode) << endl;

	switch (mode)
	{
	case SC_MODE_INIT : setup(); break;
	case SC_MODE_BEGIN : {
		easingType = EASING_SINE;
		easingInFrom = now;
		easingInTo = now + 28;
		easingOutFrom = ofGetWidth();
		easingOutTo = 0;
		
	} break;
	case SC_MODE_LINE: {
		
		easingType = EASING_EXPO;
		easingInFrom = now;
		easingInTo = now + 0.5f;
		easingOutFrom = 0;
		easingOutTo = PI;
		
					   } break;
	case SC_MODE_BEGIN_REOPEN: {
		
		easingType = EASING_BACK_EASEOUT;
		easingInFrom = now;
		easingInTo = now + 2.0f;
		easingOutFrom = 0;
		easingOutTo = ofGetWidth()*0.3;
		
							   } break;
	case SC_MODE_CLOSE : {
		
		easingType = EASING_QUAD;
		easingInFrom = now;
		easingInTo = now + 10.0f;
		easingOutFrom = ofGetWidth()*0.3;
		easingOutTo = 0;
		
						 } break;
	case SC_MODE_VOPEN : {
		sWidth = ofGetWidth();
		sHeight = 0;
		
		easingType = EASING_QUAD;
		easingInFrom = now;
		easingInTo = now + 10.0f;
		easingOutFrom = 0;
		easingOutTo = ofGetHeight();
		
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

	float now = ofGetElapsedTimef();
	int w = ofGetWidth();
	int h = ofGetHeight();

	int backColor = 0;

	// dimensions update
	switch (mode)
	{
	default :
		sWidth = getEasingValue(); break;
	case SC_MODE_VOPEN :
		sHeight = getEasingValue(); break;
	}


	// other updates (alpha, color, mode change)
	switch (mode)
	{
	case SC_MODE_BEGIN : if (now > easingInTo) nextMode(); break;
	case SC_MODE_LINE : // same as begin but color
		{
			backColor = (int)(abs(sin(2.f*sWidth))*255);
			if (now > easingInTo) nextMode();
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
		ofDrawRectangle(0, 0, lineWidth, 5);
					   } break;
	case SC_MODE_VOPEN: {
		if (openFromBottom) {
			ofTranslate(0, (h-sHeight));
		} else {
			ofTranslate(0, 0);
		}
		ofDrawRectangle(0, 0, w, sHeight);
						} break;
	default: {
		ofTranslate((w - sWidth)/2, 0);
		ofDrawRectangle(0, 0, sWidth, h);
			 } 
			 break;
	}

	ofPopMatrix();


}

//--------------------------------------------------------------
float ofxSquareScreen::getEasingValue() {
	
	float val = 0;
	float now = ofGetElapsedTimef();
	switch (easingType) {
		case EASING_CUBIC:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::cubic::easeInOut);
			break;
		case EASING_CUBIC_EASEOUT:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::cubic::easeOut);
			break;
		case EASING_SINE:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::sine::easeInOut);
			break;
		case EASING_SINE_EASEOUT:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::sine::easeOut);
			break;
		case EASING_LINEAR:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::linear::easeInOut);
			break;
		case EASING_LINEAR_EASEOUT:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::linear::easeOut);
			break;
		case EASING_EXPO:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::exp::easeInOut);
			break;
		case EASING_EXPO_EASEOUT:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::exp::easeOut);
			break;
		case EASING_QUAD:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::quad::easeInOut);
			break;
		case EASING_QUAD_EASEOUT:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::quad::easeOut);
			break;
		case EASING_BACK:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::back::easeInOut);
			break;
		case EASING_BACK_EASEOUT:
			val = ofxeasing::map(now, easingInFrom, easingInTo, easingOutFrom, easingOutTo, &ofxeasing::back::easeOut);
			break;

			
			
			
	}
	
	return val;
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