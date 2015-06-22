//
//  ofxVasaDalleQuad.cpp
//  ofVasarely
//
//  Created by Nicole & Xavier on 17/02/2015.
//
//

#include "ofxVasaDalleQuad.h"

//--------------------------------------------------------------
void ofxVasaDalleQuad::setup() {
	setup(PROJECTOR_RESOLUTION_X*2, PROJECTOR_RESOLUTION_Y*2, 256, 5);
}

//--------------------------------------------------------------
void ofxVasaDalleQuad::setup(int screenWidth, int screenHeight, int sizeOfTiles, int maxDepthLevel){
    
    w = screenWidth;
    h = screenHeight;
    tileSize = sizeOfTiles;
    maxDepth = maxDepthLevel;
    bing.setup(screenWidth, screenHeight, sizeOfTiles);

	// Directional Lights emit light based on their orientation, regardless of their position //
	light.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
	light.setSpecularColor(ofColor(255.f, 255.f, 255.f));
	//light.setSpotlight();
	//light.setDirectional();

	// set the direction of the light
	// set it pointing from left to right -> //
	//light.set
	light.setOrientation( ofVec3f(43.1984,0,0));


	// shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess(60);

	// the light highlight of the material //
	material.setSpecularColor(ofColor(255, 255, 255, 255));
}

//--------------------------------------------------------------
void ofxVasaDalleQuad::update(){
    
    clearActors();
	addActor(ofGetMouseX(), ofGetMouseY(), ofVec2f(0));

    for(int i = 0; i< actors.size(); i++) {
        ofVec2f actor = actors[i];
        
        int depth = (int)floor(ofRandom((float)maxDepth+1));

        createDalle(actor.x, actor.y, depth);
    }
    
    vector<string> dalles2Remove;
    std::map<string,ofxVasaDalle>::iterator it;
    for (it=dalles.begin(); it!=dalles.end(); ++it){
        it->second.update();
        if (it->second.alpha <=0)
            dalles2Remove.push_back(it->first);
    }
    
    for(int i = 0; i< dalles2Remove.size(); i++) {
        string quad = dalles2Remove[i];
        dalles.erase(quad);
        bing.freeQuad(quad);
    }

	light.setPosition(mouseX, mouseY, 400);	
    
}

void ofxVasaDalleQuad::createDalle(int x, int y, int depth, int shapeType){
    
    string newQuad = bing.getDeepestQuad(x, y, depth, maxDepth);
    if (newQuad != ""){
        
        cout << "depth / quad: " << depth << " / " << newQuad << endl;
        
        if (newQuad.length() == maxDepth+bing.levelOfDetail){
            
            newQuad = newQuad.substr(0, newQuad.length()-1);
            
            createDalleFromQuadKey(newQuad+"0",shapeType);
            createDalleFromQuadKey(newQuad+"1",shapeType);
            createDalleFromQuadKey(newQuad+"2",shapeType);
            createDalleFromQuadKey(newQuad+"3",shapeType);
        }
        else {
            createDalleFromQuadKey(newQuad,shapeType);
        }
    }
    else {
        //cout << "getDeepestQuad FAIL" << endl;
    }

}
void ofxVasaDalleQuad::createDalle(int x, int y, int depth){
    createDalle(x, y, depth, VASA_DALLE_SHAPE_UNDEFINED);
}

void ofxVasaDalleQuad::createDalleFromQuadKey(string key, int shapeType){
    
    if (bing.tryTakeQuad(key)){
        ofVec2f px = bing.QuadKeyToPixelXY(key, 0);
        
        ofxVasaDalle dalle = ofxVasaDalle();
        double tileSizeAtZoom = tileSize * pow(0.5, (double)key.length() - bing.levelOfDetail);
        //cout << tileSize<< endl;
        dalle.setup(tileSizeAtZoom);
        dalle.x = px.x;
        dalle.y = px.y;
        if (shapeType>VASA_DALLE_SHAPE_UNDEFINED) {
            dalle.shapeType = shapeType;
        }
        dalles.insert(std::pair<string, ofxVasaDalle>(key, dalle));
        
        //cout << " (type = " << dalle.shapeType << ")" << endl;
    } else {
        cout << " -- DENIED -- "<<endl;
        
    }
}

//--------------------------------------------------------------
void ofxVasaDalleQuad::clear(){
    dalles.clear();
    bing.reset();
    clearActors();
    
    cout << "------- ofxVasaDalleQuad cleared ----------" << endl;
}

//--------------------------------------------------------------
void ofxVasaDalleQuad::draw(){
    
	ofEnableDepthTest();

	if (bLights) {
		ofEnableLighting();
		//ofDisableLighting();
		light.enable();
		material.begin();
	}
    
    ofClear(ofColor::white);    
    std::map<string,ofxVasaDalle>::iterator it;
    for (it=dalles.begin(); it!=dalles.end(); ++it){
        it->second.draw();
    }    
    ofSetColor(ofColor::black);

	if (bLights) {
		//light.draw();
		material.end();
		ofDisableLighting();
	}
	ofDisableDepthTest();
}

void ofxVasaDalleQuad::addActor(int x, int y, ofVec2f velocity){
    if (!dynamicPathEnabled)
        return;
    
    actors.push_back(ofVec2f(x,y));
    velocities.push_back(velocity);
}

void ofxVasaDalleQuad::clearActors(){
    actors.clear();
    velocities.clear();
}

//--------------------------------------------------------------
void ofxVasaDalleQuad::keyPressed(int key){


	switch(key)
	{
	case 'l': bLights = !bLights; break;
	case 'c': clear(); break;
	case 's' : ofSetSmoothLighting(false);
	case 'S' : ofSetSmoothLighting(true);
	case 'v' : dynamicPathEnabled = !dynamicPathEnabled; break;
	case '1': {clear(); createDalle(640,400,0,VASA_DALLE_SHAPE_SQUARE); } break;
	case '2': {clear(); createDalle(640,400,0,VASA_DALLE_SHAPE_CIRCLE); } break;
	case '3': createDalle(ofRandomWidth(),ofRandomHeight(),0); break; // taken
	case '4': createDalle(ofRandomWidth(),ofRandomHeight(),(int)ofRandom(5)); break; // taken
	case 'm' : ofHideCursor(); break;
	case 'M' : ofShowCursor(); break;

	}
}