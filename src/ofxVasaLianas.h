//
//  ofxVasaLianas.h
//  ofAlReves
//
//  Created by Xavier Fischer on 25/02/2017.
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

#include "ofxLiana.h"
#include "ofxNode.h"
#include "ofxSpring.h"

const string LIANAS_SETTINGS_FILE = "lianas.xml";

class ofxVasaLianas : public ofxScene
{
public:
	ofxVasaLianas(string prefix = "scene") : ofxScene(prefix + ": " + "VasaLianas") {
		setSingleSetup(false); // call setup each time the scene is loaded
	}

	void setup();
	void update();
	void updateKinect();
	void draw();
	void drawKinect();
	void exit();
	void updateExit();
	void closeKinect();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);


private:

	void initGui();
	void initKinect();
	
	void setupKinectWarp(bool shift, bool alt, int x, int y);
	

	/*-------------------------------------------------------*/
	/* LIANAS */
	void initLianas();

	vector<ofxLiana*> lianas; 
	ofParameterGroup zebraParams;
	ofParameter<bool> randomNodes, lockX, lockY, lockZ, kinectWarp, easyCamMouse;
	ofParameter<float> kwScaleX, kwScaleY, kwX, kwY;

	ofParameter<bool> lockLastNode;
	ofParameter<int> numLianas;
	ofParameter<float> gravity;
	ofParameter<float> springLineWidth;

	ofParameterGroup nodeParams;
	ofParameter<int> numNodes;
	ofParameter<float> nodeRadius;
	ofParameter<float> nodeStrength;
	ofParameter<float> nodeDiameter;
	ofParameter<float> nodeDamping;
	ofParameter<float> nodeRamp;
	ofParameter<float> nodeVelocity;

	ofParameterGroup springParams;
	ofParameter<float> springLength;
	ofParameter<float> springStiffness;
	ofParameter<float> stringDamping;

	ofParameterGroup repulsionParams;
	ofParameter<float> repulsionRadius;
	ofParameter<float> repulsionStrength;
	bool bRepulse;
	bool bShowGui;


	/*-------------------------------------------------------*/
	/* KINECT STUFF */
	ofxKinect kinect;

	ofImage grayImage;
	ofImage grayImageNear;
	ofImage grayImageFar;
	ofxCv::ContourFinder contourFinder;

	ofImage grayImageFiltered;

	//ofEasyCam cam;

	bool bKinectFrameReady;
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
	ofParameter<bool> bShowDepth, bShowFilteredDepth;
	ofParameter<float> maximumDistance;
	ofParameter<float> persistence;


	ofParameterGroup cvGroup;
	ofParameterGroup debugGroup;

	map<int, list<vector<cv::Point> > > actors;
	map<int, ofPolyline> actorsHulls;
};

