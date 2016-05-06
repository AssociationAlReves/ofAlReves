//
//  ofxKinecticon.h
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
#include "ofxOpenCv.h"

#ifdef USE_KINECT
#include "ofxKinect.h"
#endif

#define VASA_KINECT_NUM_BG_FRAMES 200
#define VASA_KINECT_POINTCLOUD_STEP 6
#define VASA_KINECT_FRAMECOUNT 1
#define VASA_KINECT_FRAMEDELAY 20

#define VASA_KINECT_ARMNUMFRAMES 1

class ofxKinecticon : public ofxScene
{
public:
	ofxKinecticon(string prefix = "scene") : ofxScene(prefix + ": " + "Kinecticon") {
		setSingleSetup(false); // call setup each time the scene is loaded
	}

	void setup();
	void update();
	void draw();
	void drawPointCloud();
	void exit();

	void keyPressed(int key);

	// ---------------------------------------------
	// kinect related suff
	bool kinectEnabled;
	bool bKinectSetup;
	bool bShowHelp;

	ofxPanel gui;	 

#ifdef USE_KINECT
	ofxKinect kinect;
	vector<ofMesh> kinectMeshes; // meshes for kinect delayed point cloud (see https://vimeo.com/89527246 )
	vector<ofColor> kinectMeshColors;
	int kinectMeshCurColor;


	ofxCvColorImage colorImg;

	ofxCvGrayscaleImage liveImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image

	ofxCvGrayscaleImage grayBgImage; // the average background image
	ofxCvGrayscaleImage grayDiff;
	ofxCvContourFinder contourFinder;

	ofParameter<int> nearThreshold;
	ofParameter<int> farThreshold;
	ofParameter<int> threshold;
	ofParameter<bool> bKinectUseBg;
	
	ofParameter<float> repeatMsLeft;	
	ofParameter<float> repeatMsRight;
	
	ofParameter<int> center;
	ofParameter<int> margin;

	bool bLearnBackground;
	int numFramesBg;
	bool bDrawPointCloud;
	bool bKinectAltColor;
	bool showGui;

	int angle;

	ofSoundPlayer playerCCRight;
	ofSoundPlayer playerKickLeft;
	ofSoundPlayer playerCompteurCenter;


	int lastArm;
	int arm;
	int numFrames;
#endif
};

