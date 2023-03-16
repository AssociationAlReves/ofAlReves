//
//  ofxVasaDalle.cpp
//  ofVasarely
//
//  Created by Xavier Fischer on 15/02/2015.
//
//

#include "ofMain.h"
#include "ofxVasaDalle.h"

float fadeoutRate = 0.03; // 0.05
float lifeTime = 3.5; // 8.5

//--------------------------------------------------------------
void ofxVasaDalle::setup(int dalleSize){

	createdIndex = 0;
	createdTime = ofGetElapsedTimef();
	alpha = 1;
	x = y = 0;
	size = dalleSize;
	midSize = size / 2.;
	shapeSize = size * 0.8;
	seed = ofRandomf();

	if (blackAndWhite) {
		squareColor = ofColor::black;
		shapeColor = ofColor::white;
	} else {
		ofRandomize(palette);
		squareColor = palette[0];
		shapeColor = palette[1];
		squareColor.setSaturation(200);
		shapeColor.setSaturation(200);
        squareColor.setBrightness(255);
        shapeColor.setBrightness(255);

	}
	//squareColor.setBrightness(255);
	//shapeColor.setBrightness(255);

	shapeType = floorf(ofRandom(2));

	box.set(size,size,1, 2, 2, 1);
	//cout << "Shape: " << ofToString(shapeType) <<endl;

	// shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess( 120 );

}

void ofxVasaDalle::update(bool skipLifeTimeCheck){

	if (skipLifeTimeCheck) {
		createdTime = ofGetElapsedTimef()-lifeTime;
	}
	float speedFactor = 0.2;
	boxHeight = ofNoise(ofGetElapsedTimef()*speedFactor + x*0.1 + 2.56 * seed-11.4,ofGetElapsedTimef()*speedFactor+ y*0.2 + 1.23*seed + 14.6)*15*alpha;
	shapeHeight = ofNoise(ofGetElapsedTimef()*speedFactor + x*0.2 - 3.56 * seed-1.4,ofGetElapsedTimef()*speedFactor+ y*0.1 - 3.23*seed - 4.6)*(VASA_DALLE_SHAPE_HEIGHT_RATIO * size * alpha);

	if (!skipLifeTimeCheck && ofGetElapsedTimef()-createdTime>lifeTime){
		alpha = ofClamp(alpha-fadeoutRate, 0, 1);
	}

	box.setDepth(boxHeight);
	//box.set(size,size,boxHeight, 4, 4, 4);
	//cout<<ofToString(boxHeight) << endl;
}

void ofxVasaDalle::draw(){

	ofFill();

	squareColor.a = 1;
	shapeColor.a = 1;

	material.begin();
	material.setAmbientColor(squareColor);
	material.setSpecularColor(ofColor::white);
	material.setDiffuseColor(squareColor);

	squareColor.a = alpha;
	ofSetColor(squareColor);

	ofPushMatrix();
	ofTranslate(midSize, midSize);
	//box.set(size,size,boxHeight, 4, 4, 4);
	box.setPosition(x,y,0);
	box.draw();
	//ofRect(0,0,size,size);
	//ofPopMatrix();
	material.end();

	material.begin();
	if (!blackAndWhite) {
		shapeColor.a = alpha; // because black base becomes visible while fading out
	}
	material.setAmbientColor(shapeColor);
	material.setSpecularColor(ofColor::white);
	material.setDiffuseColor(shapeColor);

	
	ofSetColor(shapeColor);
	//ofPushMatrix();
	int zOffset = 0.1;
	ofTranslate(x,y,boxHeight/2+.1+shapeHeight/2);
	switch (shapeType) {
	case VASA_DALLE_SHAPE_CIRCLE:

		ofRotateDeg(90, 1, 0, 0);
		ofCylinderPrimitive(shapeSize/2, shapeHeight, 25, 1).draw();
		//ofCircle(0, 0, shapeSize / 2.0);

		break;
	case VASA_DALLE_SHAPE_SQUARE:
		ofTranslate(0,0,0);
		ofBoxPrimitive(shapeSize, shapeSize, shapeHeight,2,2,1).draw();
		//ofRect(0, 0, shapeSize, shapeSize);
		break;
	default:
		ofTranslate(0,-shapeSize*0.15,0);
		ofRotateDeg(90, 1, 0, 0);
		ofRotateDeg(90, 0, 1, 0);
		ofCylinderPrimitive(shapeSize*0.65, shapeHeight, 3, 1).draw();
		break;
	}
	ofPopMatrix();

	material.end();

}
