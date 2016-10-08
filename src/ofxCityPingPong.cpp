//
//  ofxCityPingPong.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 08/10/2016.
//

#include "ofMain.h"
#include "ofApp.h"
#include "ofxCityPingPong.h"

int rect1Vis[PING_PONG_NUM_STEPS] = { 1, 0, 0, 0, 1 };
int rect2Vis[PING_PONG_NUM_STEPS] = { 0, 1, 0, 0, 1 };
int rect3Vis[PING_PONG_NUM_STEPS] = { 0, 0, 1, 0, 1 };
int rect4Vis[PING_PONG_NUM_STEPS] = { 0, 0, 0, 1, 1 };

//--------------------------------------------------------------
void ofxCityPingPong::setup() {

	rectSize = 400;
	curMargin = 90;

	for (int i = 0; i < 4; i++) {
		rects.push_back(ofRectangle(0, 0, 100, 100));
		rectsVisible.push_back(true);
	}
	updateRects();

	

}

//--------------------------------------------------------------
void ofxCityPingPong::update() {
	rectsVisible[0] = ofGetKeyPressed('7');
	rectsVisible[1] = ofGetKeyPressed('9');
	rectsVisible[2] = ofGetKeyPressed('1');
	rectsVisible[3] = ofGetKeyPressed('3');
}

//--------------------------------------------------------------
void ofxCityPingPong::updateRects() {

	for (int i = 0; i < 4; i++) {
		rects[i].width = rectSize;
		rects[i].height = rectSize;
		rectsVisible[i] = true;
	}
	// 1 HG
	rects[0].x = curMargin;
	rects[0].y = curMargin;

	// 2 HD
	rects[1].x = ofGetScreenWidth() - curMargin - rectSize;
	rects[1].y = curMargin;

	// 3 BG
	rects[2].x = curMargin;
	rects[2].y = ofGetScreenHeight() - curMargin - rectSize;;

	// 4 BD
	rects[3].x = ofGetScreenWidth() - curMargin - rectSize;
	rects[3].y = ofGetScreenHeight() - curMargin - rectSize;


}

//--------------------------------------------------------------
void ofxCityPingPong::draw() {

	ofEnableAlphaBlending();
	ofBackground(0);

	ofFill();
	ofSetColor(255);
	//ofDrawAxis(500);

	if (bSpaceMode) {
		if (rect1Vis[curRectVisIndex] == 1)
			ofRect(rects[0]);
		if (rect2Vis[curRectVisIndex] == 1)
			ofRect(rects[1]);
		if (rect3Vis[curRectVisIndex] == 1)
			ofRect(rects[2]);
		if (rect4Vis[curRectVisIndex] == 1)
			ofRect(rects[3]);

	}
	else {
		for (int i = 0; i < 4; i++) {
			if (rectsVisible[i]) {
				ofRect(rects[i]);
			}
		}
	}


	if (bShowHelp) {

		ofApp *app = (ofApp *)ofxGetAppPtr();
		app->cam.end();

		stringstream ss;
		ss << "z/s : block sizes (curSize: " << rectSize << ")" << endl;
		ss << "q/d : margin (curMargin: " << curMargin << ")" << endl;
		ss << "command by space: " << bSpaceMode << " (step=" << curRectVisIndex  << ")" << endl;
		ofDrawBitmapStringHighlight(ss.str(), 10, 10);
		app->cam.begin();
	}


	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofxCityPingPong::keyPressed(int key) {

	int delta = 10;
	switch (key) {
	case 'h':
		bShowHelp = !bShowHelp;
		break;
	case 'q':
		curMargin = ofClamp(curMargin - delta, 0, 1000);
		updateRects();
		break;
	case 'd':
		curMargin = ofClamp(curMargin + delta, 0, 1000);
		updateRects();
		break;
	case 'z':
		rectSize = ofClamp(rectSize + delta, 0, 1000);
		updateRects();
		break;
	case 's':
		rectSize = ofClamp(rectSize - delta, 0, 1000);
		updateRects();
		break;
	case 'm':
		bSpaceMode = !bSpaceMode;
		cout << "Spacemode: " << bSpaceMode << endl;
		break;
	case ' ':
		curRectVisIndex = (curRectVisIndex + 1) % PING_PONG_NUM_STEPS;
		break;
	}
}
