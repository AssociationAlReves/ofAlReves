#include "ofMovingSquares.h"

unsigned delay = 0;

//--------------------------------------------------------------
void ofMovingSquares::setup(){

	isRotating = false;

	currentMode = 0;
	currentAngle = 0;

	shapes.clear();
	timeTriggers.clear();

	ofResetElapsedTimeCounter();
	updateTween(tweenFadein, easingsine,ofxTween::easeInOut,0,255);

#ifdef MOV_SQUARES_DEBUG
	int curTime = 0;

	timeTriggers[ofMovingSquares::state_BlackToBlank] = MOV_SQUARE_DEF_NO_TIME;
	timeTriggers[ofMovingSquares::state_StartFadeIn] = MOV_SQUARE_DEF_NO_TIME;
	curTime += 5; timeTriggers[ofMovingSquares::state_StartTimer] = curTime;
	curTime += 30; timeTriggers[ofMovingSquares::state_Slow] = curTime;
	curTime += 6; timeTriggers[ofMovingSquares::state_Accelerate] = curTime;
	curTime += 19; timeTriggers[ofMovingSquares::state_Noise] = curTime;
	curTime += 6; timeTriggers[ofMovingSquares::state_FullStop] = curTime;
	curTime += 37; timeTriggers[ofMovingSquares::state_Reset] = curTime;
	curTime += 3; timeTriggers[ofMovingSquares::state_NoGreen] = curTime;
	curTime += 4; timeTriggers[ofMovingSquares::state_NoViolet] = curTime;
	curTime += 3; timeTriggers[ofMovingSquares::state_NoBlue] = curTime;
	curTime += 40; timeTriggers[ofMovingSquares::state_NoYellow] = curTime;
	curTime += 30; timeTriggers[ofMovingSquares::state_NoRed] = MOV_SQUARE_DEF_NO_TIME;
#else
	timeTriggers[ofMovingSquares::state_BlackToBlank] = MOV_SQUARE_DEF_NO_TIME;
	timeTriggers[ofMovingSquares::state_StartFadeIn] = MOV_SQUARE_DEF_NO_TIME;
	timeTriggers[ofMovingSquares::state_StartTimer] = 195;		// start chrono for song							- 195 s
	timeTriggers[ofMovingSquares::state_Slow		] = 214;	// start slow motion - 195s (3:15)					-  19 s
	timeTriggers[ofMovingSquares::state_Accelerate	] = 220;	// start slow-> fast - 214s (3:34)					-   6 s
	timeTriggers[ofMovingSquares::state_Noise		] = 239;	// start fast w accelerating noise - 220s (3:40)		-  19 s
	timeTriggers[ofMovingSquares::state_FullStop	] = 245;	// start going to full stop - 239s (3:59)			-   6 s
	timeTriggers[ofMovingSquares::state_Reset		] = 282;	// reset to initial state - 245s (4:05)				-   6 s
	timeTriggers[ofMovingSquares::state_NoGreen		] = 286;	// green goes away - 282s (4:42)						-  37 s
	timeTriggers[ofMovingSquares::state_NoViolet	] = 289.5;	// violet goes away - 285s (4:45)					-   3 s
	timeTriggers[ofMovingSquares::state_NoBlue		] = 292.5;	// blue goes away - 289s (4:49)						-   4 s
	timeTriggers[ofMovingSquares::state_NoYellow	] = 332.5;	// yellow goes away - 292s (4:52)					-   3 s
	timeTriggers[ofMovingSquares::state_NoRed	] = MOV_SQUARE_DEF_NO_TIME;		 // red goes away - 332s (5:32)		-  40 s										// red goes away - 332s (5:32)

#endif							  		

	// -----------------------------------
	// red circle
	ShapeDef red = ShapeDef("red", ofPoint(250,250)
		, 0
		, 3*PI/4
		, 130
		, ofColor::red
		, ofMovingSquares::circle);

	// -----------------------------------
	// yellow
	ShapeDef yellow = ShapeDef("yellow", ofPoint(200,400)
		, 0
		, -PI/3
		, 200
		, ofColor::yellow
		, ofMovingSquares::triangle);

	// -----------------------------------
	// blue
	ShapeDef blue = ShapeDef("blue", ofPoint(75,400)
		, PI/3
		, PI/2
		, 400
		, ofColor::blue
		, ofMovingSquares::rectangle);

	// -----------------------------------
	// green
	ShapeDef green = ShapeDef("green", ofPoint(125,125)
		, 0
		, PI/3+0.2
		, 140
		, ofColor::green
		, ofMovingSquares::rectangle);

	// -----------------------------------
	// fuchsia
	ShapeDef fuchsia = ShapeDef("fuchsia", ofPoint(150,200)
		, PI/6
		, -3*PI/4
		, 140
		, ofColor::fuchsia
		, ofMovingSquares::parallelogram);


	shapes.push_back(red);
	shapes.push_back(yellow);
	shapes.push_back(blue);
	shapes.push_back(green);
	shapes.push_back(fuchsia);

}

//--------------------------------------------------------------
void ofMovingSquares::update(){

	if (ofGetElapsedTimef() > timeTriggers[currentMode])
	{
		nextMode("time trigger");
	}

}

void ofMovingSquares::nextMode(std::string reason){
	currentMode+=1; // next mode

	if (currentMode == timeTriggers.size()) {
		setup();
	}

	cout << " at time : " 
		<< ofToString(floorf(ofGetElapsedTimef()/60.0))
		<< ":" + ofToString(ofGetElapsedTimef() - 60*floorf(ofGetElapsedTimef()/60.0))
		<< " -> new mode: " + ofToString(currentMode) 
		<< ", reason: " << reason 
		<< endl;

	switch(currentMode) {
	case ofMovingSquares::state_BlackToBlank:
		updateTween(tweenFadein, easingsine,ofxTween::easeInOut,0,255);
		break;
	case ofMovingSquares::state_StartFadeIn:
		updateTween(tweenFadein, easingsine,ofxTween::easeInOut,0,255);
		break;
	case ofMovingSquares::state_Slow:
		{
			isRotating = true;
			updateTween(tweenFadein, easingsine,ofxTween::easeOut,0,2,1000);
		}
		break;
	case ofMovingSquares::state_Accelerate:
		{
			isRotating = true;
			updateTween(tweenFadein, easinglinear,ofxTween::easeInOut,2,20,5000);
		}
		break;
	case ofMovingSquares::state_Noise:
		{
			isRotating = true;
			updateTween(tweenNoise, easingcubic,ofxTween::easeInOut,0,1,5000);
		}
		break;
	case ofMovingSquares::state_FullStop:
		{
			isRotating = true;
			updateTween(tweenFadein, easingsine,ofxTween::easeInOut,20,0,3000);
			updateTween(tweenNoise, easingsine,ofxTween::easeInOut,1,0,3000); // 6000
		}
		break;
	case ofMovingSquares::state_Reset:
		{
			isRotating = true;
			isStopFadeOut = true;
			updateTween(tweenNoise, easinglinear,ofxTween::easeInOut,255,0,200);
		}
		break;
	case ofMovingSquares::state_NoGreen:
		{
			/* contents should be
			shapes.push_back(red); 0
			shapes.push_back(yellow);1
			shapes.push_back(blue);  2
			shapes.push_back(green); 3
			shapes.push_back(fuchsia);4*/
			shapes.erase(shapes.begin()+3);
		}
		break;
	case ofMovingSquares::state_NoViolet:
		{
			/* contents should be
			shapes.push_back(red); 0
			shapes.push_back(yellow);1
			shapes.push_back(blue);  2
			shapes.push_back(fuchsia);3*/
			shapes.erase(shapes.begin()+3);
		}
		break;
	case ofMovingSquares::state_NoBlue:
		{
			/* contents should be
			shapes.push_back(red); 0
			shapes.push_back(yellow);1
			shapes.push_back(blue);  2*/
			shapes.erase(shapes.begin()+2);
		}
		break;
	case ofMovingSquares::state_NoYellow:
		{
			/* contents should be
			shapes.push_back(red); 0
			shapes.push_back(yellow);1*/
			shapes.erase(shapes.begin()+1);
		}
		break;
	case ofMovingSquares::state_NoRed:
		{
			/* contents should be
			shapes.push_back(red); 0*/
			shapes.clear();
		}
		break;
	case ofMovingSquares::state_StartTimer: 
		{
			ofResetElapsedTimeCounter();
		}
		break;
	}
}

//--------------------------------------------------------------
bool ofMovingSquares::beforeDraw(){

	bool bDrawShapes = true;
	switch(currentMode) {
	case ofMovingSquares::state_BlackToBlank: 
		{
			ofClear(tweenFadein.update());
			bDrawShapes = false;
		}
		break;
	default:
		{
			bDrawShapes = true;
			ofClear(255);
		}
		break;
	}

	return bDrawShapes;
}

//--------------------------------------------------------------
void ofMovingSquares::draw(){

	float adjustedTime = 0;
	float amout = 0;
	float noiseAmount = 0;

	if (beforeDraw()){

		cam.begin();

		ofPushMatrix();
		ofSetCircleResolution(50);

		ofScale (1,-1,1);

		float posAngle = 0;
		float shapeAngle = 0;
		if (isRotating) {

			adjustedTime = ofGetElapsedTimef()-timeTriggers[ofMovingSquares::state_StartTimer];
			amout = tweenFadein.isCompleted() ? tweenFadein.getTarget(0) : tweenFadein.update();
			currentAngle += 0.003 * amout;

			posAngle = currentAngle * -0.1;
			shapeAngle = currentAngle * 4.f;
		}
		else
		{
			amout = tweenFadein.getTarget(0);
		}
		//rects.push_back(ofRectangle(-w/2,-h/2,w,h));
		int i = 0;
		for(std::vector<ShapeDef>::iterator shapeIt = shapes.begin(); shapeIt != shapes.end(); ++shapeIt) {

			i++;

			ShapeDef shape = *shapeIt;
			//--------------------------------------------------------------
			// shape color alpha
			if (currentMode == ofMovingSquares::state_StartFadeIn) {
				ofColor color = shape.color;
				color.a  = tweenFadein.update();
				ofSetColor(color);
			} else if (currentMode == ofMovingSquares::state_Reset) {
				ofColor color = shape.color;
				color.a  = tweenNoise.update();
				if (tweenNoise.isCompleted() && tweenNoise.getTarget(0) == 0) {
					isStopFadeOut = false;
					isRotating = false;
					updateTween(tweenNoise, easinglinear,ofxTween::easeInOut,0,255,200);
				}
				ofSetColor(color);
			} else {
				ofSetColor(shape.color);
			}
			ofPushMatrix();

			//--------------------------------------------------------------
			// shape position
			switch (currentMode)
			{
			case ofMovingSquares::state_Noise:
				{
					float curMargin = shape.margin;//+ 100 * ofSignedNoise(ofGetElapsedTimef()*0.2+i);
					float a = shape.angleOffset + posAngle;
					float nw = ofGetWidth() / 2;
					float nh = ofGetHeight() / 2;
					float speed = 0.08;
					noiseAmount = tweenNoise.update();
					// standard elliptic rotation
					ofTranslate((1. - noiseAmount*0.8) * cos( shape.angleOffset + posAngle ) * ( ofGetWidth() / 2 - curMargin)
						, (1. - noiseAmount*0.8) * sin( shape.angleOffset + posAngle ) * ( ofGetHeight() / 2 - curMargin));
					// perlin noise based motion
					ofTranslate(noiseAmount * nw * ofSignedNoise((adjustedTime+i)*speed + i * 0.123+4.5)
						, noiseAmount * nh * ofSignedNoise((adjustedTime+i)*speed + i * 0.456+1.23));

					// shape rotation on its axis
					ofRotateZ(shapeAngle);
					ofRotateZ(shapeAngle * noiseAmount*0.2 * ofSignedNoise((adjustedTime+i)*speed*5. + i * 0.987+1.23));; 
				}
				break;
			case ofMovingSquares::state_FullStop:
				{
					float curMargin = shape.margin;//+ 100 * ofSignedNoise(ofGetElapsedTimef()*0.2+i);
					float a = shape.angleOffset + posAngle;
					float nw = ofGetWidth() / 2;
					float nh = ofGetHeight() / 2;
					float speed = 0.08;
					noiseAmount = tweenNoise.update();
					// standard elliptic rotation
					ofTranslate((1. - 1*0.8) * cos( shape.angleOffset + posAngle) * ( ofGetWidth() / 2 - curMargin)
						, (1. - 1*0.8) * sin( shape.angleOffset + posAngle) * ( ofGetHeight() / 2 - curMargin));
					// perlin noise based motion
					ofTranslate(noiseAmount * nw * ofSignedNoise((adjustedTime+i)*speed + i * 0.123+4.5)
						, noiseAmount * nh * ofSignedNoise((adjustedTime+i)*speed + i * 0.456+1.23));

					// shape rotation on its axis
					ofRotateZ(shapeAngle);
					ofRotateZ(shapeAngle * noiseAmount*0.2 * ofSignedNoise((adjustedTime+i)*speed*5. + i * 0.987+1.23));
				}
				break;
			case ofMovingSquares::state_Reset:
				{
					if (isStopFadeOut) {
						float curMargin = shape.margin;//+ 100 * ofSignedNoise(ofGetElapsedTimef()*0.2+i);
						float a = shape.angleOffset + posAngle;
						float nw = ofGetWidth() / 2;
						float nh = ofGetHeight() / 2;
						float speed = 0.08;
						noiseAmount = 0; //tweenNoise.update();
						// standard elliptic rotation
						ofTranslate((1. - 1*0.8) * cos( shape.angleOffset + posAngle) * ( ofGetWidth() / 2 - curMargin)
							, (1. - 1*0.8) * sin( shape.angleOffset + posAngle) * ( ofGetHeight() / 2 - curMargin));
						// perlin noise based motion
						ofTranslate(noiseAmount * nw * ofSignedNoise((adjustedTime+i)*speed + i * 0.123+4.5)
							, noiseAmount * nh * ofSignedNoise((adjustedTime+i)*speed + i * 0.456+1.23));

						// shape rotation on its axis
						ofRotateZ(shapeAngle);
						ofRotateZ(shapeAngle * noiseAmount*0.2 * ofSignedNoise((adjustedTime+i)*speed*5. + i * 0.987+1.23));
					}
					else {
						float curMargin = shape.margin;//+ 100 * ofSignedNoise(ofGetElapsedTimef()*0.2+i);
						ofTranslate(cos(shape.angleOffset + posAngle) * ( ofGetWidth() / 2 - curMargin)
							, sin(shape.angleOffset + posAngle) * ( ofGetHeight()/2 - curMargin));

						// shape rotation on its axis
						ofRotateZ(shapeAngle);
					}
				}
				break;
			default:
				{
					float curMargin = shape.margin;//+ 100 * ofSignedNoise(ofGetElapsedTimef()*0.2+i);
					ofTranslate(cos(shape.angleOffset + posAngle) * ( ofGetWidth() / 2 - curMargin)
						, sin(shape.angleOffset + posAngle) * ( ofGetHeight()/2 - curMargin));

					// shape rotation on its axis
					ofRotateZ(shapeAngle);
				}
				break;
			}

			// shape static rotation
			ofRotateZ(shape.orientation * 180.0/PI);

			//--------------------------------------------------------------
			// shape draw
			int w = shape.size.x;
			int h = shape.size.y;
			ofTranslate(0,0,i);
			switch (shape.shapeType)
			{
			case ofMovingSquares::rectangle:
				ofRect(-w/2,-h/2,w,h);
				break;
			case ofMovingSquares::circle:
				ofCircle(0,0, w/2);
				break;

			case ofMovingSquares::parallelogram:
				ofBeginShape();
				ofVertex(-w/2, 0);
				ofVertex(0, -h/2);
				ofVertex(w/2, 0);
				ofVertex(0, h/2);
				ofEndShape();
				break;

			case ofMovingSquares::triangle:
				// rect triangle
				ofTriangle(-w/2,-h/2,
					w/2,-h/2,
					w/2,h/2);
				// isocele
				/*ofTriangle(-w/2,-h/2,
				w/2,-h/2,
				0,h/2);*/
				break;

			}


			ofPopMatrix();
		}

		ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);


		ofPopMatrix();

		//ofDrawAxis(200);
		afterDraw();

		cam.end();
	}

#ifdef _DEBUG
	//def MOV_SQUARES_DEBUGd
	ofSetColor(ofColor::black);
	string strMode = "";
	switch (currentMode) {
	case ofMovingSquares::state_BlackToBlank: strMode = "modeBlackToBlank"; break; //0;	// initial screen
	case ofMovingSquares::state_StartFadeIn: strMode = "modeStartFadeIn"; break; //1;	// before song
	case ofMovingSquares::state_StartTimer: strMode = "modeStartTimer"; break; //2;	// start chrono for song - 0s
	case ofMovingSquares::state_Slow: strMode = "modeSlow"; break; //3;			// start slow motion - 195s (3:15)
	case ofMovingSquares::state_Accelerate: strMode = "modeAccelerate"; break; //4;	// start slow-> fast - 214s (3:34)
	case ofMovingSquares::state_Noise: strMode = "modeNoise"; break; //5;			// start fast w accelerating noise - 220s (3:40)
	case ofMovingSquares::state_FullStop: strMode = "modeFullStop"; break; //6;		// start going to full stop - 239s (3:59)
	case ofMovingSquares::state_Reset: strMode = "modeReset"; break; //7;			// reset to initial state - 245s (4:05)

	case ofMovingSquares::state_NoGreen: strMode = "modeNoGreen"; break; //8;		// green goes away - 282s (4:42)
	case ofMovingSquares::state_NoViolet: strMode = "modeNoViolet"; break; //9;		// violet goes away - 285s (4:45)
	case ofMovingSquares::state_NoBlue: strMode = "modeNoBlue"; break; //10;		// blue goes away - 289s (4:49)
	case ofMovingSquares::state_NoYellow: strMode = "modeNoYellow"; break; //11;		// yellow goes away - 292s (4:52)
	case ofMovingSquares::state_NoRed: strMode = "modeNoRed"; break; //12;		// red goes away - 332s (5:32)
	}
	int posY = 5;
	stringstream ss;
	ss << "current mode : " + strMode << endl;
	ss << "time : " + ofToString(floorf(ofGetElapsedTimef()/60.0)) + ":" + ofToString(ofGetElapsedTimef() - 60*floorf(ofGetElapsedTimef()/60.0)) << endl;
	ss << "adj. time : " + ofToString(adjustedTime) << endl;
	ss << "amout : " + ofToString(amout) << endl;
	ss << "noise amout : " + ofToString(noiseAmount) << endl;
	ss << "FPS : " + ofToString(ofGetFrameRate());
	ofDrawBitmapStringHighlight(ss.str(), 10, 10);
#endif
}

//--------------------------------------------------------------
void ofMovingSquares::afterDraw(){
	switch(currentMode) {
	case ofMovingSquares::state_StartFadeIn: 
		{
			/*ofSetColor(tweenFadein.update());
			ofRect(0,0,ofGetWidth(),ofGetHeight());*/
		}
		break;
	default:
		{
			//ofClear(255);
		}
		break;
	}


}

//--------------------------------------------------------------
void ofMovingSquares::keyPressed(int key){
	switch (key)
	{
	case ' ': 
		// force change mode
		// update all time triggers to match the time offset
		if (timeTriggers[currentMode] != MOV_SQUARE_DEF_NO_TIME){
			float deltaTime = timeTriggers[currentMode] - ofGetElapsedTimef();
			for (int i = currentMode; i < timeTriggers.size(); i++) {
				timeTriggers[i] -= deltaTime;
			}

			cout << "adjusted time triggers: t = t - " << ofToString(deltaTime) << endl;
		}
		nextMode("keyPressed");
		break;
	case 'r':
		setup(); 
		break;
	case OF_KEY_UP:
		break;
	case OF_KEY_DOWN:
		break;
	case OF_KEY_LEFT:
		break;
	case OF_KEY_RIGHT:
		break;
	default:
		break;
	}
}


void ofMovingSquares::updateTween(ofxTween & _tween, ofxEasing & _easing, ofxTween::ofxEasingType _type,  float _from, float _to, float _duration) {
	cout << "changed tween" << endl;
	_tween.setParameters(1,_easing,_type, _from,_to,_duration, delay);
	_tween.start();
}
