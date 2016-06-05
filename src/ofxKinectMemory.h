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

class ofxKinectMemory : public ofxScene
{
public:
	ofxKinectMemory(string prefix = "scene") : ofxScene(prefix + ": " + "KinectMemory") {
		setSingleSetup(false); // call setup each time the scene is loaded
	}

	void setup();
	void update();
	void draw();
	void drawPointCloud();
	void drawConvexHulls();
	void exit();

	void keyPressed(int key);

	ofxKinect kinect;

	ofImage grayImage;	
	ofImage grayImageNear;
	ofImage grayImageFar;	
	ofxCv::ContourFinder contourFinder;

	ofImage grayImageFiltered;

	//ofEasyCam cam;

	bool bGotImage;
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


	ofParameterGroup cvGroup;
	ofParameterGroup appGroup;
	ofParameterGroup debugGroup;

	map<int,list<vector<cv::Point>>> actors;	
	map<int,ofPolyline> actorsHullUnion;

	ofFbo fbo;

};

