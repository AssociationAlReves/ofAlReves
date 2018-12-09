//
//  ofxOcean.cpp
//  ofxOcean
//
//  Created by Xavier Fischer on 28/12/2015.
//
//

#include "ofxOcean.h"
#include "ofApp.h"

//--------------------------------------------------------------
void ofxOcean::setup(){
    
	ofApp *app = (ofApp *)ofxGetAppPtr();
//    app->transformer.cam.reset();
//    
//    app->transformer.cam.lookAt(ofVec3f(0.151753, 0.923233, -0.353005));
//    app->transformer.cam.setPosition(ofVec3f(-600.906, -3368.04, 538.046));
//    
    
    // turn on smooth lighting //
    smoothLighting     = true;
    ofSetSmoothLighting(true);
    
    // Point lights emit light in all directions //
    // set the diffuse color, color reflected from the light source //
    // specular color, the highlight/shininess color //
    
    pointLight.setPointLight();
    
    // Directional Lights emit light based on their orientation, regardless of their position //
    directionalLight.setDirectional();
    
    // set the direction of the light
    // set it pointing from left to right -> //
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess(shininess);
    // the light highlight of the material //
    material.setColors(matDiffuseColor, matAmbientColor, matSpecularColor, matEmissiveColor);

    
    width = 1000;
    height = 1000;
    resolution = 100;
    gridSize = width / resolution;
    
    setupMesh(width,height,gridSize);
    
    gui.setup("panel",Globals::hostName + OCEAN_SETTINGS_FILE); // most of the time you don't need a name but don't forget to call setup
    gui.add(filled.set("bFill", true));
    gui.add(useLights.set("Use lights", true));
    gui.add(smoothLighting.set("Smooth Lighting", false));
    gui.add(usePointLight.set("usePointLight", false));
    gui.add(useDirLight.set("useDirLight", true));
    gui.add(useMaterial.set("useMaterial", true));
    
    meshParams.setName("Ocean mesh");
    meshParams.add(meshColor.set("meshColor", ofFloatColor(1, 0.72449, 1, 1),ofFloatColor(0,0,0,0),ofColor::white));
    meshParams.add(meshSize.set("size", 10, 10,100));
    meshParams.add(meshResolution.set("resolution", 2, 1,10));
    meshParams.add(noiseAmp.set("noiseAmp", 2, 0,50));
    meshParams.add(noiseSpeed.set("noiseSpeed", 2, 0,50));
    meshParams.add(noiseHeight.set("noiseHeight", 2, 0,500));
    meshParams.add(waveAmp.set("waveAmp", 0,0,PI));
    meshParams.add(waveSpeed.set("waveSpeed", 0,0,10));
    meshParams.add(waveHeight.set("waveHeight", 0,0,500));
    meshParams.add(waveDirection.set("waveDirection", ofVec3f(0),ofVec3f(-1),ofVec3f(1)));
    meshParams.add(waveAmp2.set("waveAmp2", 0,0,PI));
    meshParams.add(waveSpeed2.set("waveSpeed2", 0,0,10));
    meshParams.add(waveHeight2.set("waveHeight2", 0,0,500));
    meshParams.add(waveDirection2.set("waveDirection2", ofVec3f(0),ofVec3f(-1),ofVec3f(1)));
    
    gui.add(meshParams);
    
    lightParams.setName("Lights");
    lightParams.add(dirLightOrientation.set("Orientation (x,z,y angles)", ofVec3f(180, -42.2449, 117.551), ofVec3f(-180), ofVec3f(180)));
    lightParams.add(diffuseColor.set("diffuseColor", ofFloatColor(1, 1, 1, 1),ofFloatColor(0,0,0,0),ofColor::white));
    lightParams.add(specularColor.set("specularColor", ofFloatColor(1, 1, 1, 1), ofFloatColor(0,0,0,0),ofColor::white));
    lightParams.add(ambientColor.set("ambientColor", ofFloatColor(0, 0.00510204, 1, 1), ofFloatColor(0,0,0,0),ofColor::white));
    gui.add(lightParams);
    
    materialParams.setName("Material");
    materialParams.add(shininess.set("shininess", 128, 0, 128));
    materialParams.add(matDiffuseColor.set("matDiffuseColor", ofFloatColor(0, 0.204082, 1, 1), ofFloatColor(0,0,0,0),ofColor::white));
    materialParams.add(matSpecularColor.set("matSpecularColor", ofFloatColor(1, 1, 1, 1), ofFloatColor(0,0,0,0),ofColor::white));
    materialParams.add(matEmissiveColor.set("matEmissiveColor", ofFloatColor(0, 0.540816, 1, 1), ofFloatColor(0,0,0,0),ofColor::white));
    materialParams.add(matAmbientColor.set("matAmbientColor", ofFloatColor(0, 0.55102, 1, 1), ofFloatColor(0,0,0,0),ofColor::white));
    gui.add(materialParams);
    bShowGui = false;
    
}

//--------------------------------------------------------------
void ofxOcean::updateMesh(float W, float H, int res){
    if (width != W || height != H || resolution != res) {
        width = W; height = H; resolution = res; gridSize = W / res;
        setupMesh(W, H, gridSize);
    }
}

//--------------------------------------------------------------
void ofxOcean::setupMesh(float W, float H, int gridSize){
    
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    mesh.clear();
    mesh.enableColors();
    mesh.enableIndices();
    mesh.enableNormals();
    mesh.disableTextures();
    
    //Set up vertices and colors
    bool odd = true;
    float offset = 0;
    for (int y=0; y<H; y+=gridSize) {
        for (int x=0; x<W; x+=gridSize) {
            
            if (odd) {
                if (x == 0) {
                    //mesh.addVertex(ofPoint(0, y, 0));
                    //mesh.addColor( ofColor( 0, 0, 0 ) );
                }
                offset = gridSize/2.0;
            } else {
                offset = 0;
            }
            mesh.addVertex(ofPoint(x+offset, y, 0));
            mesh.addColor( ofColor(255.f, 255.f, 255.f, 64.f) );
        }
        if (!odd){
            //mesh.addVertex(ofPoint(W-gridSize/2, y, 0));
            //mesh.addColor( ofColor( 0, 0, 0 ) );
        }
        odd = !odd;
    }
    
    //Set up triangles' indices
    odd = true;
    int numRows = H/gridSize;
    int numCols = W/gridSize;

    for (int y=0; y<numRows-1; y++) {
        for (int x=0; x<numCols-1; x++) {
            int i1 = x + numCols * y;
            int i2 = x+1 + numCols * y;
            int i3 = x + numCols * (y+1);
            int i4 = x+1 + numCols * (y+1);
            
            if (odd) {
                mesh.addTriangle( i1, i4, i3 );
                mesh.addTriangle( i1, i2, i4 );
            } else {
                mesh.addTriangle( i1, i2, i3 );
                mesh.addTriangle( i2, i4, i3 );
            }
        }
        odd = !odd;
    }
    setNormals( mesh );  //Set normals
    
}

//--------------------------------------------------------------
void ofxOcean::update(){

    float radius = 500;
    ofVec3f center = ofVec3f(0,0,-500);
    pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x,
                           sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y,
                           -cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);
    
    Filled = filled;
    
    updateMesh(meshSize*100, meshSize*100, MAX(1, (int)meshResolution)*10);
    NoiseAmp = noiseAmp;
    NoiseHeight = noiseHeight;
    NoiseSpeed = noiseSpeed;
    WaveSpeed = waveSpeed;
    WaveHeight = waveHeight;
    WaveDirection = ((ofVec3f)waveDirection).normalize();
    WaveAmplitude = waveAmp;
    WaveSpeed2 = waveSpeed2;
    WaveHeight2 = waveHeight2;
    WaveDirection2 = ((ofVec3f)waveDirection2).normalize();
    WaveAmplitude2 = waveAmp2;
    
    
    // z = sin(y)
    
    int W = width/gridSize;
    int H = height/gridSize;
    float time = ofGetElapsedTimef();
    //Change vertices
    for (int y=0; y<H; y++) {
        
        //float waveZ = sin(y * waveAmplitude + time * waveSpeed) * waveHeight;
        
        for (int x=0; x<W; x++) {
        
            float waveZ = sin((y*WaveDirection.y + x*WaveDirection.x)  * WaveAmplitude + time * WaveSpeed) * WaveHeight;
            waveZ += sin((y*WaveDirection2.y + x*WaveDirection2.x)  * WaveAmplitude2 + time * WaveSpeed2) * WaveHeight2;
            
            int i = x + W * y;       //Vertex index
            ofPoint p = mesh.getVertex( i );
            //Get Perlin noise value
            float value =
            ofNoise( x * gridSize * noiseAmp * 0.001, y * gridSize * noiseAmp * 0.001, time * noiseSpeed * 0.2 ); // 0.2
            //Change z-coordinate of vertex
            p.z = value * noiseHeight; //25
        
            p.z += waveZ;
            
            mesh.setVertex( i, p );
            //Change color of vertex
            mesh.setColor( i, Color);
                    }
        
    }
    setNormals( mesh );  //Update the normals
    
    
    
    directionalLight.setDiffuseColor(diffuseColor);
    directionalLight.setSpecularColor(specularColor);
    directionalLight.setAmbientColor(ambientColor);
    
    pointLight.setDiffuseColor(diffuseColor);
    pointLight.setSpecularColor(specularColor);
    pointLight.setAmbientColor(ambientColor);
    
    directionalLight.setDirectional();
    directionalLight.setOrientation((ofVec3f)dirLightOrientation);
    
    material.setShininess(shininess);
    material.setColors(matDiffuseColor, matAmbientColor, matSpecularColor, matEmissiveColor);
    
    ofApp *app = (ofApp *)ofxGetAppPtr();

//    cout << "llokAt" << app->transformer.cam.getLookAtDir() << endl;
//
//    cout << "pos" << app->transformer.cam.getPosition() << endl;
    
}

//--------------------------------------------------------------
void ofxOcean::draw(){
	ofApp *app = (ofApp *)ofxGetAppPtr();
	//app->transformer.cam.end();

    ofBackgroundGradient(ofColor::fromHex(0xFFFFFF), ofColor::fromHex(0x82CAFF));
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    
    
    // enable the material, so that it applies to all 3D objects before material.end() call //
    if (useMaterial) {
        material.begin();
    }
    
    if (useLights) {
        ofEnableLighting();
        ofSetSmoothLighting(smoothLighting);
        // activate the lights //
        if (usePointLight && useLights) pointLight.enable();
        if (useDirLight && useLights) directionalLight.enable();
    } else {
        ofDisableLighting();
    }
    
    

//    app->transformer.cam.begin();
//
//
//    app->transformer.cam.setFarClip(100000);

    
    ofPushMatrix();
    ofTranslate(-width/2, -height/2, -500);
    ofTranslate(0,1000);
    
//    mesh.setMode( OF_PRIMITIVE_TRIANGLES );
//    vector<ofMeshFace> triangles = mesh.getUniqueFaces();
//    mesh.setFromTriangles(triangles, false);

    if (filled) {
        mesh.drawFaces();
        //mesh.draw();
    } else {
        mesh.drawWireframe();
    }
    
    ofPopMatrix();

	//app->transformer.cam.end();
    
    if (useLights) {
        // activate the lights //
        if (!usePointLight) pointLight.disable();
        if (!useDirLight) directionalLight.disable();
    }
    
    
    // turn off lighting //
    ofDisableLighting();
    material.end();
    
    
    if (bShowGui) {
        ofDisableDepthTest();
        gui.draw();
    }
	//app->transformer.cam.begin();
}


//--------------------------------------------------------------

//Universal function which sets normals for the triangle mesh
void ofxOcean::setNormals( ofMesh &mesh ){
    //The number of the vertices
    int nV = mesh.getNumVertices();
    //The number of the triangles
    int nT = mesh.getNumIndices() / 3;
    vector<ofPoint> norm( nV ); //Array for the normals
    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (int t=0; t<nT; t++) {
        //Get indices of the triangle t
        int i1 = mesh.getIndex( 3 * t );
        int i2 = mesh.getIndex( 3 * t + 1 );
        int i3 = mesh.getIndex( 3 * t + 2 );
        //Get vertices of the triangle
        const ofPoint &v1 = mesh.getVertex( i1 );
        const ofPoint &v2 = mesh.getVertex( i2 );
        const ofPoint &v3 = mesh.getVertex( i3 );
        //Compute the triangle's normal
        ofPoint dir = ( (v2 - v1).getCrossed( v3 - v1 ) ).getNormalized();
        //Accumulate it to norm array for i1, i2, i3
        norm[ i1 ] += dir;
        norm[ i2 ] += dir;
        norm[ i3 ] += dir;
    }
    //Normalize the normal's length
    for (int i=0; i<nV; i++) {
        norm[i].normalize();
    }
    //Set the normals to mesh
    mesh.clearNormals();
   // mesh.addNormals.addNormals( norm );
}

//--------------------------------------------------------------
void ofxOcean::keyPressed(int key){
    ofApp *app = (ofApp *)ofxGetAppPtr();

    
    if( key == 'h' ){
        bShowGui = !bShowGui;
				if (bShowGui){
			//app->transformer.cam.disableMouseInput();
		}
		else {
			//app->transformer.cam.enableMouseInput();
		}
    }
    if (key =='<') {
        waveHeight -= 10;
        waveHeight = ofClamp(waveHeight, 0, 500);
    }
    if (key =='>') {
        waveHeight += 10;
    }
    if(key == 's') {
        gui.saveToFile(OCEAN_SETTINGS_FILE);
    }
    if(key == 'l') {
        gui.loadFromFile(OCEAN_SETTINGS_FILE);
        
    }
   
}

