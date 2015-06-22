//
//  ofMovingSquares.h
//  ofAlReves
//
//  Created by Xavier Fischer on 21/06/2015.
//
//
#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxAppUtils.h"
#include "ofxTween.h"

#define MOV_SQUARES_MIN_SIZE 50.f
#define MOV_SQUARES_SMOOTH 0.01f
//#define MOV_SQUARES_DEBUG
#define MOV_SQUARE_DEF_NO_TIME 99999

#ifdef MOV_SQUARES_DEBUG
#define MOV_SQUARE_DURATION 1000
#else
#define MOV_SQUARE_DURATION 3000
#endif

#define MOV_state_BlackToBlank		0	// initial screen
#define MOV_state_StartFadeIn		1	// before song
#define MOV_state_StartTimer		2	// start chrono for song - 0s
#define MOV_state_MoveViolet		3	// 45s
#define MOV_state_MoveGreen			4	// 53.5
#define MOV_state_MoveRed			5	// start chrono for song - 0s
#define MOV_state_MoveYellow		6	// start chrono for song - 0s
#define MOV_state_MoveBlue			7	// start chrono for song - 0s
#define MOV_state_StopMoveBlue		8	// start chrono for song - 0s
#define MOV_state_Slow				9	// start slow motion - 195s (3:15)
#define MOV_state_Accelerate		10	// start slow-> fast - 214s (3:34)
#define MOV_state_Noise				11
// start fast w accelerating noise - 220s (3:40)
#define MOV_state_FullStop			12	// start going to full stop - 239s (3:59)
#define MOV_state_Reset				13	// reset to initial state - 245s (4:05)
#define MOV_state_NoGreen			 14	// green goes away - 282s (4:42)
#define MOV_state_NoViolet			15	// violet goes away - 285s (4:45)
#define MOV_state_NoBlue			16	// blue goes away - 289s (4:49)
#define MOV_state_NoYellow			17	// yellow goes away - 292s (4:52)
#define MOV_state_NoRed				18	// red goes away - 332s (5:32)

#define MOV_rectangle		 1
#define MOV_circle			 2
#define MOV_triangle		 3
#define MOV_parallelogram	 4


// This is meant to be sync with beethoven sonata

class ofMovingSquares  : public ofxFadeScene {
private:

	struct ShapeDef {
		std::string name;
		ofPoint size;
		float orientation;
		float angleOffset;
		float margin;
		ofColor color;
		int shapeType;

		ShapeDef(){

		}
		ShapeDef(std::string name, ofPoint size, 	float orientation, float angleOffset,
					float margin, ofColor color, int shapeType)
		: name(name), size(size), orientation(orientation), angleOffset(angleOffset),margin(margin),color(color), shapeType(shapeType) {
		}
	};
    
	int currentMode;
	void nextMode(std::string reason);

	map<int, float> timeTriggers; // <state, time until next step>
	vector<ShapeDef> shapes;

	ofEasyCam cam;

	bool isRotating;
	bool isStopFadeOut;

	float currentAngle;

	// modes

	// transitions
	ofxTween tweenFadein;
	ofxTween tweenNoise;

	ofxEasingBack 	easingback;
	ofxEasingBounce 	easingbounce;
	ofxEasingCirc 	easingcirc;
	ofxEasingSine	easingsine;
	ofxEasingCubic 	easingcubic;
	ofxEasingElastic easingelastic;
	ofxEasingExpo 	easingexpo;
	ofxEasingLinear 	easinglinear;
	ofxEasingQuad 	easingquad;
	ofxEasingQuart 	easingquart;
	ofxEasingQuint 	easingquint;

	bool beforeDraw();
	void afterDraw();

	void updateTween(ofxTween & _tween, ofxEasing & _easing, ofxTween::ofxEasingType _type,  float _from, float _to, float _duration = MOV_SQUARE_DURATION );


public:
    
	ofMovingSquares() : ofxFadeScene("Moving squares"){
			setSingleSetup(true); // call setup each time the scene is loaded
			setFade(1000, 1000); // 1 second fade in/out
		}

	void setup();
	void update();
	void draw();

	void keyPressed(int key);

};