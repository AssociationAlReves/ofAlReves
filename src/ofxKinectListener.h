//
//  ofxKinectListener.h
//  ofAlReves
//
//  Created by Xavier Fischer on 08/07/2015.
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
#define VASA_KINECT_POINTCLOUD_STEP 2

class ofxKinectListener : public ofxScene
{
public:
	ofxKinectListener(string prefix = "scene") : ofxScene(prefix + ": " + "KinectListener") {
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
    
    bool bRotate;
    float minZ;
    float maxZ;

    
	ofxPanel gui;
    
	#ifdef USE_KINECT
    ofxKinect kinect;
    ofMesh kinectMesh;
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
    
    int nearThreshold;
    int farThreshold;
    int threshold;
    
    int angle;
    #endif
};

