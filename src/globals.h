#include "ofMain.h"

#pragma once

// this must match the display resolution of your projector

#if __APPLE__

#define PROJECTOR_RESOLUTION_X 1024 //1024 //1280
#define PROJECTOR_RESOLUTION_Y 768 //768 //800
#define USE_KINECT

#else // PC

//#define PROJECTOR_RESOLUTION_X 1024//1280 //512 //1024 //1280
//#define PROJECTOR_RESOLUTION_Y 768//800// 32 //768 //800
#define PROJECTOR_RESOLUTION_X 1280 //512 //1024 //1280
#define PROJECTOR_RESOLUTION_Y 800// 32 //768 //800
//#define USE_KINECT

#endif


// -----------------------
// OSC related
#define ALREVES_USE_OSC


#define EASING_LINEAR 0
#define EASING_SINE 1
#define EASING_EXPO 2
#define EASING_CUBIC 3
#define EASING_BACK 4
#define EASING_QUAD 5
#define EASING_LINEAR_EASEOUT 6
#define EASING_SINE_EASEOUT 7
#define EASING_EXPO_EASEOUT 8
#define EASING_CUBIC_EASEOUT 9
#define EASING_BACK_EASEOUT 10
#define EASING_QUAD_EASEOUT 11

// settings are stored in <hostName>osc_settings.xml
// this where master is defined or not
// can be change in Debug mode of ofxApp ('d' key)
class Globals {
public:
    
    //---------------------------
    // OSC related
    // If true, mouse and keys will be sent to slaves (1 supported now)
    static bool oscIsMaster;
    
    // mouse and keys from MASTER
    static int oscMouseX;
    static int oscMouseY;
	static int oscKeyPressed;
    
    static int oscSceneIndex;

    // flag set when first message gets in and logged in std out
	static bool oscGotMessage;

    // to prevent OF 0.9.x full screen bug where perfs are slow when calling ofGetWidth()
    static int screenWidth;
    static int screenHeight;

	static void oscGotMessageFunc();

    // hostName is used to have per-machine settings and source control not overwriting them
    static std::string hostName;
};
