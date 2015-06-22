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
    case SC_MODE_LINE :
        {
            sWidth = tween.update();
            cout << ofToString(sWidth) << endl;
            if (tween.isCompleted()) {
                nextMode();
            }
        }
            break;
    case SC_MODE_BEGIN_REOPEN :
            sWidth = tween.update(); break;
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
		case SC_MODE_CLOSE:
        case SC_MODE_BEGIN_REOPEN :{
			ofTranslate((w - sWidth)/2, 0);
			ofRect(0, 0, sWidth, h);
							} break;
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
		tween.setParameters(easingsine, ofxTween::easeInOut
			, ofGetWidth()
			, 0
			, 30000,0);
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
void ofxSquareScreen::keyPressed(int key){
	switch (key)
	{
	case ' ' : nextMode(); break;
	case 'r': setup(); break;
	default:
		break;
	}
}