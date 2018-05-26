//
//  ofBande.cpp
//  ofAlRevesTemp
//
//  Created by Xavier Fischer on 22/05/2018.
//

#include "ofBande.h"

//------------------------------------------------------------------
ofBande::ofBande(){
}

//------------------------------------------------------------------
void ofBande::setMode(bandeMode newMode){
    mode = newMode;
}

//------------------------------------------------------------------
void ofBande::reset(){
   
}

//------------------------------------------------------------------
void ofBande::update(){
    
}

//------------------------------------------------------------------
void ofBande::draw(){
    
    // 4 points polygon
    ofFill();
    ofSetColor(color);
 
    ofSetPolyMode(OF_POLY_WINDING_ODD);    // this is the normal mode
    ofBeginShape();
    ofVertex(pos.x + size.x * (1-sizeFactor.x), pos.y);
    ofVertex(pos.x + size.x * sizeFactor.x, pos.y);
    ofVertex(pos.x + size.x * sizeFactor.y, pos.y + size.y);
    ofVertex(pos.x + size.x * (1-sizeFactor.y), pos.y + size.y);
    ofEndShape();
    
}

