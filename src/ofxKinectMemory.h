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
#include "ofxUI.h"
#include "ofxOpenCv.h"
#ifdef USE_KINECT
#include "ofxKinect.h"
#endif

#define VASA_KINECT_NUM_BG_FRAMES 200
#define VASA_KINECT_POINTCLOUD_STEP 2
#define VASA_KINECT_FRAMECOUNT 100
#define VASA_KINECT_FRAMEDELAY 20

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
	void exit();

	void keyPressed(int key);

	// ---------------------------------------------
    // kinect related suff
    bool kinectEnabled;
    bool bKinectSetup;
	bool bShowHelp;

	ofxUISuperCanvas *gui;
    
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
	
    
    bool bLearnBackground;
    int numFramesBg;
    bool bDrawPointCloud;
    bool bKinectUseBg;
    bool bKinectAltColor;
    
    int nearThreshold;
    int farThreshold;
    int threshold;
    
    int angle;
    #endif
};

