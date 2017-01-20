//
//  ofxTerrain.h
//  displacementMap
//
//  Created by Xavier Fischer on 11/02/2015.
//
//
#pragma once

#define VASA_DALLE_SHAPE_HEIGHT_RATIO 0.1 // 0.2
#define VASA_DALLE_SHAPE_UNDEFINED -1
#define VASA_DALLE_SHAPE_CIRCLE 0
#define VASA_DALLE_SHAPE_SQUARE 1

#include "ofMain.h"

class ofxVasaDalle {

public:

	ofxVasaDalle(bool blackAndWhite = false) :
				blackAndWhite(blackAndWhite) {

		if (blackAndWhite)
		{
			//palette.push_back(ofColor::black); // mauve
			//palette.push_back(ofColor::white); // mauve
		} else {
			palette.push_back(ofColor::fromHex(0x8F6EC1)); // mauve
			palette.push_back(ofColor::fromHex(0x7EA878)); // vert
			palette.push_back(ofColor::fromHex(0xFEA15C)); // jaune
			palette.push_back(ofColor::fromHex(0xFF653B)); // orange
			palette.push_back(ofColor::fromHex(0x5152BE)); // violet

			palette.push_back(ofColor::fromHex(0x587390)); // vert fonce / bleu
			palette.push_back(ofColor::fromHex(0x4043C4)); // bleu profond
			palette.push_back(ofColor::fromHex(0x8188E3)); // bleu profond
			palette.push_back(ofColor::yellow); // bleu profond
			//    palette.push_back(ofColor::red); // bleu profond
		}
	}

	void setup(int dalleSize);
	void update(bool skipLifeTimeCheck);
	void draw();

	int x,y;

	int size;
	float seed;

	float alpha; // when 0, dalle is removed


	// 0: circle, 1: square, 2:triangle
	int shapeType;

private:

	float createdTime;
	vector<ofColor> palette;

	ofFloatColor squareColor;
	ofFloatColor shapeColor;

	ofBoxPrimitive box;

	ofMaterial material;

	bool blackAndWhite;

	float midSize;
	float shapeSize;

	float boxHeight;
	float shapeHeight;
};