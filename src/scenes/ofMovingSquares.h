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

	static const int rectangle		= 1;
	static const int circle			= 2;
	static const int triangle		= 3;
	static const int parallelogram	= 4;


	static const int state_BlackToBlank	= 0;	// initial screen
	static const int state_StartFadeIn	= 1;	// before song
	static const int state_StartTimer	= 2;	// start chrono for song - 0s
	static const int state_Slow			= 3;	// start slow motion - 195s (3:15)
	static const int state_Accelerate	= 4;	// start slow-> fast - 214s (3:34)
	static const int state_Noise		= 5;	// start fast w accelerating noise - 220s (3:40)
	static const int state_FullStop		= 6;	// start going to full stop - 239s (3:59)
	static const int state_Reset		= 7;	// reset to initial state - 245s (4:05)
	static const int state_NoGreen		= 8;	// green goes away - 282s (4:42)
	static const int state_NoViolet		= 9;	// violet goes away - 285s (4:45)
	static const int state_NoBlue		= 10;	// blue goes away - 289s (4:49)
	static const int state_NoYellow		= 11;	// yellow goes away - 292s (4:52)
	static const int state_NoRed		= 12;	// red goes away - 332s (5:32)

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