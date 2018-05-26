#include "ofParticles.h"
#include "ofxEasing.h"


//--------------------------------------------------------------
void ofParticles::setup(){
    ofSetVerticalSync(true);
    
    gridSize = 10;
    numX = ofGetWidth()/gridSize;
    numY = ofGetHeight()/gridSize;
    int num  = numX * numY;
    isExplosing = false;
    
    p.assign(num, demoParticle());
    currentMode = PARTICLE_MODE_REPEL;
    
    currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
    
    resetParticles();
    
    screenHeight = ofGetHeight();
    screenWidth = ofGetWidth();
    
    if (!bGuiLoaded) {
        gui.setup("cityPanel", POP_ART_PARTICLES_FILE); // most of the time you don't need a name but don't forget to call setup
        gui.add(dragForceParam.set("dragForceParam", 0.99, 0.8, 0.99));
        gui.add(forceParam.set("forceParam", 0.6, 0.5, 1));
        gui.add(returnForceParam.set("returnForceParam", 0.2, 0, 1));
        gui.add(distParam.set("distParam", 150, 50, 800));
        gui.add(resetDistThresholdParam.set("resetDistThresholdParam", 10, 0,20));
        
        bGuiLoaded = true;
    
    }
    bShowGui = false;
}

//--------------------------------------------------------------
void ofParticles::resetParticles(){
    
    //these are the attraction points used in the forth demo
    attractPoints.clear();
    for(int i = 0; i < 4; i++){
        attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, screenWidth-100) , ofRandom(100, screenHeight-100) ) );
    }
    
    attractPointsWithMovement = attractPoints;
    
    if (currentMode == PARTICLE_MODE_REPEL) {
        for (unsigned int x = 0; x < numX; x++) {
            for (unsigned int y = 0; y < numY; y++ ) {
                
                unsigned int i = x + y * numX;
                p[i].setMode(currentMode);
                p[i].setAttractPoints(&attractPointsWithMovement);;
                p[i].reset();
                
                
                p[i].pos.x = x * gridSize;
                p[i].pos.y = y * gridSize + (x % 2) * (gridSize/2.0);
                p[i].initialPos = p[i].pos;
            }
        }
    } else {
        for(unsigned int i = 0; i < p.size(); i++){
            p[i].setMode(currentMode);
            p[i].setAttractPoints(&attractPointsWithMovement);;
            p[i].reset();
        }
    }
}

//--------------------------------------------------------------
void ofParticles::update(){
    auto now = ofGetElapsedTimef();
    if (now > endTime) {
        isExplosing = false;
    }
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setMode(currentMode);
        p[i].screenWidth = screenWidth;
        p[i].screenHeight = screenHeight;
        p[i].dragForce = dragForceParam;
        p[i].force = forceParam;
        p[i].returnForce = returnForceParam;
        p[i].resetDistThreshold = resetDistThresholdParam;
        
        if (isExplosing) {
            p[i].dist = ofxeasing::map_clamp(now, initTime, endTime, distParam, screenHeight, &ofxeasing::exp::easeOut);
        } else {
            p[i].dist = distParam;
        }
        
        p[i].update();
    }
    
    //lets add a bit of movement to the attract points
    for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
        attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
        attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
    }
}

//--------------------------------------------------------------
void ofParticles::draw(){
   // ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    ofClear(255);
    
    //    cam.begin();
    //    ofPushMatrix();
    //    ofTranslate(-screenWidth/2, -screenHeight/2);
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].draw();
    }
    
    ofSetColor(190);
    if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
        for(unsigned int i = 0; i < attractPoints.size(); i++){
            ofNoFill();
            ofDrawCircle(attractPointsWithMovement[i], 10);
            ofFill();
            ofDrawCircle(attractPointsWithMovement[i], 4);
        }
    }
    
    //    ofPopMatrix();
    //    cam.end();
    
    if (bShowGui) {
        gui.draw();
    }
//    ofSetColor(230);
//    ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-4 to change mode.", 10, 20);
}

//--------------------------------------------------------------
void ofParticles::keyPressed(int key){
    if( key == '1'){
        currentMode = PARTICLE_MODE_ATTRACT;
        currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse. hold ' ' to disable force";
    }
    if( key == '2'){
        currentMode = PARTICLE_MODE_REPEL;
        currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse. hold ' ' to disable force";
    }
    if( key == '3'){
        currentMode = PARTICLE_MODE_NEAREST_POINTS;
        currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold ' ' to disable force";
    }
    if( key == '4'){
        currentMode = PARTICLE_MODE_NOISE;
        currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation";
        resetParticles();
    }
    if (key == 'e'){
        if (!isExplosing){
            // start easing explosion
            initTime = ofGetElapsedTimef();
            endTime = initTime + 0.5;
            isExplosing = true;
        }
    }
    if( key == 'r' ){
        resetParticles();
    }
    
    if (key == 'h'){
        bShowGui = !bShowGui;
    }
    
    if(key == 'f'){
        ofToggleFullscreen();
        screenHeight = ofGetHeight();
        screenWidth = ofGetWidth();
    }
    
    if (key == 'S') {
        gui.saveToFile(POP_ART_PARTICLES_FILE);
    }
    if (key == 'L') {
        gui.loadFromFile(POP_ART_PARTICLES_FILE);
    }
}

//--------------------------------------------------------------
void ofParticles::keyReleased(int key){
    if (key == 'e'){
        isExplosing = false;
    }
}

//--------------------------------------------------------------
void ofParticles::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofParticles::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofParticles::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofParticles::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofParticles::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofParticles::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofParticles::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofParticles::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofParticles::dragEvent(ofDragInfo dragInfo){
    
}
