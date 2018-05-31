#pragma once

// this must match the display resolution of your projector

#if __APPLE__

#define PROJECTOR_RESOLUTION_X 1280 //1024 //1280
#define PROJECTOR_RESOLUTION_Y 800 //768 //800
#define USE_KINECT
    
#else

//#define PROJECTOR_RESOLUTION_X 1024//1280 //512 //1024 //1280
//#define PROJECTOR_RESOLUTION_Y 768//800// 32 //768 //800
#define PROJECTOR_RESOLUTION_X 1280 //512 //1024 //1280
#define PROJECTOR_RESOLUTION_Y 800// 32 //768 //800
#define USE_KINECT

#endif


// -----------------------
// OSC related
#define ALREVES_USE_OSC
// For sender
#define HOST "169.254.242.115"
// For sender AND receiver
#define PORT 9000
// Set to 1 if hosting, 0 if receiving
#define IS_HOST 1


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

class Globals {
public:
    static int oscMouseX;
    static int oscMouseY;
	
	static int oscKeyPressed;
	
	static bool oscGotMessage;
    
    static int screenWidth;
    static int screenHeight;
	
	static void oscGotMessageFunc();
};
