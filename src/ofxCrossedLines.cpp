#include "ofxCrossedLines.h"

float duration = 5000;
float wait = 500;
//--------------------------------------------------------------
void ofxCrossedLines::setup(){
	numLine = -1;
	tween.setParameters(easingcubic, ofxTween::easeInOut, 0,1,duration,wait);
}

//--------------------------------------------------------------
void ofxCrossedLines::update(){
	if (numLine == -1) return;
	
	if (tween.isCompleted() && numLine != 4)
	{
		nextMode();
	}
}

void ofxCrossedLines::nextMode() {

	tween.setParameters(easingcubic, ofxTween::easeInOut, 0,1,duration,wait);
	numLine += 1;
	if (numLine == 5) numLine = -1;

	cout << "NumLine : " << ofToString(numLine) << endl;
}

//--------------------------------------------------------------
void ofxCrossedLines::draw(){

	ofClear(255);
	ofEnableAlphaBlending();

	ofSetColor(ofColor::red, 128);
	ofSetLineWidth(20);
	for (int i = 0; i<=numLine; i++)
	{
		if (i == numLine) {
			drawLine(i, tween.update());
		} else {
			drawLine(i);
		}
	}


	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofxCrossedLines::drawLine(int lineIndex, float interpolation){
	
	float xStart = 0, yStart = 0, xEnd = 0, yEnd = 0;
	float w = ofGetWidth(), h = ofGetHeight();
	float m = 10; // margin

	switch (lineIndex)
	{
	case 0: 
		xStart = w+m; xEnd = 0-m;
		yStart = h/4; yEnd = h/4;
		break;	
	case 1: 
		xStart = 0-m; xEnd = w+m;
		yStart = h/5; yEnd = h * 3/4;
		break;
	
	case 2: 
		xStart = w+m; xEnd = 0-m;
		yStart = h/6; yEnd = h * 5/7;
		break;
	
	case 3: 
		xStart = 0-m; xEnd = w+m;
		yStart = h * 5/6; yEnd = h * 2/3;
		break;

	}

	ofLine(xStart, yStart, 0
			, ofLerp(xStart, xEnd, interpolation), ofLerp(yStart, yEnd, interpolation), 0);
}

//--------------------------------------------------------------
void ofxCrossedLines::keyPressed(int key){
	switch (key)
	{
	case 'r': setup(); break;
	case ' ': nextMode(); break;
	default:
		break;
	}
}