//
//  ofxVasaCubeMesh.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 16/07/2015.
//
//

#include "ofxVasaCubeMesh.h"


//--------------------------------------------------------------
void ofxVasaCubeMesh::setup(){
}

//--------------------------------------------------------------
void ofxVasaCubeMesh::update(){
    
}

//--------------------------------------------------------------
void ofxVasaCubeMesh::draw(){
    ofClear(ofColor(255,255,255,alpha));
}

//--------------------------------------------------------------
void ofxVasaCubeMesh::clear(){
}

//--------------------------------------------------------------
void ofxVasaCubeMesh::mouseMoved(int x, int y ){
}



//--------------------------------------------------------------
void ofxVasaCubeMesh::keyPressed(int key){
    switch (key)
    {
        case ' ' : clear(); break;
        case 'r': setup(); break;
        default:
            break;
    }
}