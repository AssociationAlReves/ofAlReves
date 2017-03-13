//
//  ofxKinectMemory.h
//  ofAlReves
//
//  Created by Xavier Fischer on 21/06/2015.
//
//
#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxQuadWarp.h"


class ofxKinectMemory : public ofxScene
{
public:
	ofxKinectMemory(string prefix = "scene") : ofxScene(prefix + ": " + "KinectMemory") {
		setSingleSetup(false); // call setup each time the scene is loaded
	}

	void setup();
	void update();
	void draw();
	void exit();
	void updateExit();
	void closeKinect();

	void keyPressed(int key);

private:

	void drawMemoryTrails();

	ofxKinect kinect;

	ofImage grayImage;	
	ofImage grayImageNear;
	ofImage grayImageFar;	
	ofxCv::ContourFinder contourFinder;

	ofImage grayImageFiltered;

	//ofEasyCam cam;

	bool bKinectFrameReady;
	bool bShowHelp;	
	ofxPanel gui;	 
	ofParameter<float> nearThreshold;
	ofParameter<float> farThreshold;
	ofParameter<float> thresholdParam;
	ofParameter<int> blurSize;	
	ofParameter<float> contourMinArea;
	ofParameter<float> contourMaxArea;

	ofParameter<int> numFramesDelay;
	ofParameter<int> angle;

	ofParameter<bool> bShowLabels;
	ofParameter<bool> bShowImages;
	ofParameter<bool> bStartMemory;
	ofParameter<float> maximumDistance;
	ofParameter<float> persistence;
	ofParameter<float> fadeAmnt;

	ofParameter<bool> blackScreen;	
	ofParameter<bool> antiAlias;	
	ofParameter<float> lineWidth;

	ofParameter<ofColor> lineColor;

	ofParameterGroup cvGroup;
	ofParameterGroup appGroup;
	ofParameterGroup debugGroup;
    
    
    ofParameter<ofVec3f> camOrientation;
    ofParameter<ofVec3f> camPosition;

	map<int,list<vector<cv::Point> > > actors;
	map<int,ofPolyline> actorsHullUnion;
	ofPolyline bigHull2Actors;
	bool bDrawJoinedActors;

	ofFbo fboWhite;
	ofFbo fboBlack;

	// warp
	ofxQuadWarp warper;
	bool forceWarpOff;

};

