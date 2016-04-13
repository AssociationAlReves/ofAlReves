//
//  ofxOcean.h
//  ofxOcean
//
//  Created by Xavier Fischer on 28/12/2015.
//
//

#pragma once

#include "ofMain.h"
#include "globals.h"
#include "ofxTween.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"

class ofxOcean : public ofxScene
 {
    
public:
     ofxOcean(string prefix = "scene") :
     ofxScene(prefix + ": " + "Ocean") {
         setSingleSetup(false); // call setup each time the scene is loaded
     }
    void setup();
    void update();
    void updateMesh(float W, float H, int size);
    void draw();
     void keyPressed(int key);
    
    bool Filled;
    ofFloatColor Color;
    float NoiseAmp;
    float NoiseSpeed;
    float NoiseHeight;
    
    ofVec3f WaveDirection;
    float WaveHeight;
    float WaveAmplitude;
    float WaveSpeed;
    
    ofVec3f WaveDirection2;
    float WaveHeight2;
    float WaveAmplitude2;
    float WaveSpeed2;


private:
    void setupMesh(float W, float H, int res);
    
    ofVboMesh mesh;
    int width;
    int height;
    int gridSize;
    int resolution;
    
    //Universal function which sets normals for the triangle mesh
    void setNormals( ofMesh &mesh );
     // transitions
     ofxTween tween;
     
     ofxPanel gui;
     ofParameter<bool> useLights;
     ofParameter<bool> filled;
     ofParameter<bool> smoothLighting;
     ofParameter<bool> usePointLight;
     ofParameter<bool> useDirLight;
     ofParameter<bool> useMaterial;
     ofParameter<float> shininess;
     
     // lights params
     ofParameter<ofVec3f> dirLightOrientation;
     ofParameter<ofFloatColor> diffuseColor;
     ofParameter<ofFloatColor> specularColor;
     ofParameter<ofFloatColor> ambientColor;
     ofParameterGroup lightParams;
     
     // material params
     ofParameter<ofFloatColor> matAmbientColor;
     ofParameter<ofFloatColor> matDiffuseColor;
     ofParameter<ofFloatColor> matSpecularColor;
     ofParameter<ofFloatColor> matEmissiveColor;
     ofParameterGroup materialParams;
     
     // mesh params
     ofParameter<ofFloatColor> meshColor;
     ofParameter<int> meshSize;
     ofParameter<int> meshResolution;
     ofParameter<float> noiseAmp;
     ofParameter<float> noiseSpeed;
     ofParameter<float> noiseHeight;
     
     ofParameter<float> waveAmp;
     ofParameter<float> waveSpeed;
     ofParameter<float> waveHeight;
     ofParameter<ofVec3f> waveDirection;
     ofParameter<float> waveAmp2;
     ofParameter<float> waveSpeed2;
     ofParameter<float> waveHeight2;
     ofParameter<ofVec3f> waveDirection2;
     ofParameterGroup  meshParams;
     
     bool bShowGui;
     
     
     ofLight directionalLight;
     ofLight pointLight;
     
     ofMaterial material;



};
