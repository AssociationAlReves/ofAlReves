//
//  ofxVasaDalle.cpp
//  ofVasarely
//
//  Created by Xavier Fischer on 15/02/2015.
//
//

#include "ofMain.h"
#include "ofxVasaDalle.h"

float fadeoutRate = 0.05; // 0.05
float lifeTime = 10.5;

ofxVasaDalle::ofxVasaDalle(){
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

//--------------------------------------------------------------
void ofxVasaDalle::setup(int dalleSize){

    createdTime = ofGetElapsedTimef();
    alpha = 1;
    x = y = 0;
    size = dalleSize;
    midSize = size / 2.;
    shapeSize = size * 0.8;
    ofRandomize(palette);
    seed = ofRandomf();
    
    squareColor = palette[0];
    shapeColor = palette[1];
    squareColor.setSaturation(200);
    shapeColor.setSaturation(200);
    //squareColor.setBrightness(255);
    //shapeColor.setBrightness(255);
    
	shapeType = floorf(ofRandom(2));
    
    box.set(size,size,1, 2, 2, 1);
    //cout << "Shape: " << ofToString(shapeType) <<endl;
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 120 );
    
}

void ofxVasaDalle::update(){
    
    boxHeight = ofNoise(ofGetElapsedTimef()+seed)*20;
    shapeHeight = ofNoise(ofGetElapsedTimef()*1.-5.+seed)*(VASA_DALLE_SHAPE_HEIGHT_RATIO * size);

    if (ofGetElapsedTimef()-createdTime>lifeTime){
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
    material.setAmbientColor(shapeColor);
    material.setSpecularColor(ofColor::white);
    material.setDiffuseColor(shapeColor);
    
    shapeColor.a = alpha;
    ofSetColor(shapeColor);
    //ofPushMatrix();
    int zOffset = 0.1;
    ofTranslate(x,y,boxHeight/2+.1+shapeHeight/2);
    switch (shapeType) {
        case VASA_DALLE_SHAPE_CIRCLE:
            
            ofRotate(90, 1, 0, 0);
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
            ofRotate(90, 1, 0, 0);
            ofRotate(90, 0, 1, 0);
            ofCylinderPrimitive(shapeSize*0.65, shapeHeight, 3, 1).draw();
            break;
    }
    ofPopMatrix();
    
    material.end();
    
}