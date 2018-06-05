//
//  ofxTunnel.h
//  ofAlReves
//
//  Created by Xavier Fischer on 03/04/2018.
//

#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxKinect.h"

const string TUNNEL_SETTINGS_FILE = "tunnel_settings.xml";

class ofxTunnel : public ofxScene
{
public:
    ofxTunnel(string prefix = "scene") : ofxScene(prefix + ": " + "VasaTunnel") {
        setSingleSetup(false); // call setup each time the scene is loaded
        guiInitialized = false;
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
    void windowResized(int w, int h);
    
private:
    
    float screenWidth, screenHeight;
    bool guiInitialized;
    void initKinect();
    
    bool bUseMouseRepulsor;
    bool bUseKinectRepulsor;
    
    void setupKinectWarp(bool shift, bool alt, int x, int y);
    
    /*-------------------------------------------------------*/
   
    ofParameter<bool>  kinectWarp, easyCamMouse;
    ofParameter<float> kwScaleX, kwScaleY, kwX, kwY;
    ofParameter<ofVec2f> screenBounds;
    ofParameter<ofVec2f> screenTopLeftPos;
    
    bool bShowGui;
    void initGui();
    ofxPanel gui;
    /*-------------------------------------------------------*/
    /* TUNNEL STUFF */
    ofParameter<int> tunnelSteps;
    ofParameter<float> tunnelSpeed; // 100
    ofParameter<float> tunnelHueSpeed; // 0.01
    ofParameter<float> tunnelSaturationSpeed; // 0.0034
    ofParameter<float> tunnelBrightnessSpeed; // 0.001
    ofParameter<float> tunnelRotation;
    //ofParameter<int> tunnelSteps;
    
    ofParameterGroup tunnelGroup;
    
    void drawTunnelPart(float x, float y, float z, float w, float h, float bandWidth, ofColor const & color);
    
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
  
    /*-------------------------------------------------------*/
    /* GUI STUFF */
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

