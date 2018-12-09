//
//  ofxCityPingPong.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 08/10/2016.
//

#include "ofMain.h"
#include "ofApp.h"
#include "ofxCityPingPong.h"

/////////////////////////////////////////////////////////////// 1  2  3  4  5  6  7  8/ 1  2  3  4  5  6  7  8                   
int rect1Vis[PING_PONG_NUM_STEPS] = { 1,0,0,0 };
int rect2Vis[PING_PONG_NUM_STEPS] = { 0,0,0,0 };
int rect3Vis[PING_PONG_NUM_STEPS] = { 0,0,0,0 };
int rect4Vis[PING_PONG_NUM_STEPS] = { 0,1,0,0 };

//--------------------------------------------------------------
void ofxCityPingPong::setup() {

    if (!guiInitialized) {
        initGui();
    }
    
	ofApp *app = (ofApp *)ofxGetAppPtr();
    app->transformer.resetWarp();

	curRectVisIndex = 0;

	rects.clear();
	rectsVisible.clear();
	for (int i = 0; i < 4; i++) {
		rects.push_back(ofRectangle(0, 0, 100, 100));
		rectsVisible.push_back(true);
	}
	updateRects();
}
//--------------------------------------------------------------
void ofxCityPingPong::initGui() {
    
    gui.setup("panel", Globals::hostName + PING_PONG_SETTINGS_FILE); // most of the time you don't need a name but don't forget to call setup
    
    gui.add(ppSize.set("Size z/s",400,0,1000));
    gui.add(ppMargin.set("Margin q/d",160,0,800));

    bShowGui = false;
    guiInitialized = true;
    
}

//--------------------------------------------------------------
void ofxCityPingPong::update() {
	rectsVisible[0] = ofGetKeyPressed('7');
	rectsVisible[1] = ofGetKeyPressed('9');
	rectsVisible[2] = ofGetKeyPressed('1');
	rectsVisible[3] = ofGetKeyPressed('3');
}
void ofxCityPingPong::altRects(){
    rect1Vis[0] = 1;
    rect1Vis[1] = 0;
    
    rect4Vis[0] = 0;
    rect4Vis[1] = 1;
}
void ofxCityPingPong::normalRects(){
    rect1Vis[0] = 0;
    rect1Vis[1] = 1;
    
    rect4Vis[0] = 1;
    rect4Vis[1] = 0;
}

//--------------------------------------------------------------
void ofxCityPingPong::updateRects() {

	for (int i = 0; i < 4; i++) {
		rects[i].width = ppSize;
		rects[i].height = ppSize;
		rectsVisible[i] = true;
	}

    // 1 BD
	rects[0].x = ofGetScreenWidth() - ppMargin - ppSize;
	rects[0].y = ofGetScreenHeight() - ppSize;

	// 2 HG
	rects[1].x = ppMargin;
	rects[1].y = 0;

	// 3 HD
	rects[2].x = ofGetScreenWidth() - ppMargin - ppSize;
	rects[2].y = 0;

	// 4 BG
	rects[3].x = ppMargin;
	rects[3].y = ofGetScreenHeight() - ppSize;


}

//--------------------------------------------------------------
void ofxCityPingPong::draw() {

	ofEnableAlphaBlending();
	ofBackground(0);

	ofFill();
	ofSetColor(255);


	if (bSpaceMode) {
		if (rect1Vis[curRectVisIndex] == 1)
			ofDrawRectangle(rects[0]);
		if (rect2Vis[curRectVisIndex] == 1)
			ofDrawRectangle(rects[1]);
		if (rect3Vis[curRectVisIndex] == 1)
			ofDrawRectangle(rects[2]);
		if (rect4Vis[curRectVisIndex] == 1)
			ofDrawRectangle(rects[3]);

	}
	else {
		for (int i = 0; i < 4; i++) {
			if (rectsVisible[i]) {
				ofDrawRectangle(rects[i]);
			}
		}
	}


	if (bShowHelp || bShowGui) {

		ofDisableDepthTest();
		ofApp *app = (ofApp *)ofxGetAppPtr();
		//app->transformer.cam.end();

		stringstream ss;
		ss << "z/s : block sizes (curSize: " << ppSize << ")" << endl;
		ss << "q/d : margin (curMargin: " << ppMargin << ")" << endl;
		ss << "command by space: " << bSpaceMode << " (step=" << curRectVisIndex << ")" << endl;
        ss << "i/I : top/bottom screen" << endl;
        ofDrawBitmapStringHighlight(ss.str(), 10, 10);

            gui.draw();

		//app->transformer.cam.begin();

		ofEnableDepthTest();
	}


	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofxCityPingPong::keyPressed(int key) {

	int delta = 10;
	switch (key) {
	case 'r':
		setup();
		break;
        case 'i':
            altRects();
            break;
        case 'I':
            normalRects();
            break;
	case 'h':
		bShowHelp = !bShowHelp;
		break;
	case 'q':
		ppMargin = ofClamp(ppMargin - delta, 0, 1000);
		updateRects();
		break;
	case 'd':
		ppMargin = ofClamp(ppMargin + delta, 0, 1000);
		updateRects();
		break;
	case 'z':
		ppSize = ofClamp(ppSize + delta, 0, 1000);
		updateRects();
		break;
	case 's':
		ppSize = ofClamp(ppSize - delta, 0, 1000);
		updateRects();
		break;
    case 'S': gui.saveToFile(Globals::hostName + PING_PONG_SETTINGS_FILE);
        break;
	case 'm':
		bSpaceMode = !bSpaceMode;
		cout << "Spacemode: " << bSpaceMode << endl;
		break;
	case ' ':
		curRectVisIndex = (curRectVisIndex + 1) % PING_PONG_NUM_STEPS;
		break;
        case 'L':
            gui.loadFromFile(Globals::hostName + PING_PONG_SETTINGS_FILE);
            break;
	
	}
}
