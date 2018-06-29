#include "ofxRibbon.h"
#include "ofApp.h"


//--------------------------------------------------------------
void ofxRibbon::setup(){
	sWidth = ofGetWidth();
	sHeight = ofGetHeight();

	parts.clear();
	
    if (bGuiLoaded == false){
        gui.setup("Ribbon control", Globals::hostName + RIBBON_SETTINGS_FILE);
        
        gui.add(speedParam.set("speed",2,0,20));
        gui.add(rangeMinParam.set("range min",-2000,-5000,0));
        gui.add(rangeMaxParam.set("range max",-500,-1000,0));
        gui.add(snakeSpeedParam.set("snake speed",0,10,3));
        gui.add(snakeStepsParam.set("snake steps",1,10,4));
        gui.loadFromFile(Globals::hostName + RIBBON_SETTINGS_FILE);
        
        bGuiLoaded = true;
    }
    
    parts.assign(10, ofxRibbonPart());
    parts[0].mouseDriven = true;
    
    mode = RIBBON_MODE_FREE;

}

//--------------------------------------------------------------
void ofxRibbon::update(){
    
#ifdef ALREVES_USE_OSC
    if (Globals::oscKeyPressed != 0){
        bool isSpaceKeyPressed = Globals::oscKeyPressed == ' ';
        keyPressed(Globals::oscKeyPressed);
        Globals::oscKeyPressed = 0; // reset for non repeating
        cout << "popup space key" << endl;
    }
#endif
    
    if (mode == RIBBON_MODE_FREE){
        for (int i = 0; i< parts.size(); i++){
#ifdef ALREVES_USE_OSC
            if (Globals::oscIsMaster == false){
                if (parts[i].mouseDriven){
                    parts[i].addPoint(Globals::oscMouseX, Globals::oscMouseY);
                }
            }
#endif
        }
    } else if (mode == RIBBON_MODE_HLINE){
        // easing for lines
        // bounce or elastic are ok
        
        now = ofGetElapsedTimef();
        if (now <= endTime){
            
            float easingValue = ofxeasing::map(now, startTime, endTime, 0, Globals::screenWidth / 2.0+50, &ofxeasing::linear::easeOut);
            
            parts[0].addPoint(Globals::screenWidth/2 + easingValue, Globals::screenHeight/2);
            parts[1].addPoint(Globals::screenWidth/2 - easingValue, Globals::screenHeight/2);
            
        }
    } else if (mode == RIBBON_MODE_CIRCLE){
        
        now = ofGetElapsedTimef();
        
        float easingValue = ofxeasing::map(now, startTime, endTime, 0, 2.0*PI, &ofxeasing::linear::easeIn);
        
        float radius = (2.0*Globals::screenHeight/3)/2.0;
        parts[0].addPoint(Globals::screenWidth/2 + cos(easingValue) * radius, Globals::screenHeight/2 + sin(easingValue) * radius);
        
    } else if (mode == RIBBON_MODE_SNAKE){
        
        float duration = 8;
        now = ofGetElapsedTimef();
        if (now >= startTime+duration){
            startTime = now;
        }
        
        float easingValue = ofxeasing::map(now, startTime, startTime+duration, 0, 1, &ofxeasing::linear::easeInOut);
        
        float sinVal = sin(easingValue*2.0*PI);
        float xOffset = sinVal * Globals::screenWidth/2.0;
        
        if (abs(sinVal)>0.8){
            float yFactor = ofMap(1-abs(sinVal),0.2,0,-PI/2.0,PI/2.0);
            yFactor = 1+sin(yFactor)/2.0;
            yOffset -= yFactor * 2.0;
            cout << yFactor << endl;
           // yOffset += 10.0* sin(ofMap(1-abs(sinVal),0.2,0,0,PI));
        }
        //cout << easingValue << endl;
        float radius = (2.0*Globals::screenHeight/3)/2.0;
        parts[0].addPoint(Globals::screenWidth/2 + xOffset, Globals::screenHeight - 50 + yOffset);
        
    } else if (mode == RIBBON_MODE_HLINE_BOTTOM){
        // easing for lines
        
        now = ofGetElapsedTimef();
        if (now <= endTime){
            
            float easingValue = ofxeasing::map(now, startTime, endTime, 0, Globals::screenWidth / 2.0+50, &ofxeasing::linear::easeOut);
            
            parts[0].addPoint(Globals::screenWidth/2 + easingValue, Globals::screenHeight * 0.2);
            parts[1].addPoint(Globals::screenWidth/2 - easingValue, Globals::screenHeight * 0.2);
            
        }
    } else if (mode == RIBBON_MODE_VLINES){
        // easing for lines
        
        now = ofGetElapsedTimef();
        if (now <= endTime){
            
            float easingValue = ofxeasing::map(now, startTime, endTime, 0, Globals::screenHeight, &ofxeasing::linear::easeInOut);
            
            cout << easingValue << endl;
            float spacing = Globals::screenWidth/11.0;
            for (int i = 0; i< parts.size(); i++){
                parts[i].addPoint(spacing * (i+1), easingValue);
            }
            
        }
    }
    
    for (int i = 0; i< parts.size(); i++){
        
        parts[i].rangeMaxParam = rangeMaxParam;
        parts[i].rangeMinParam = rangeMinParam;
        parts[i].speedParam = speedParam;
        parts[i].update();
    }
}

//--------------------------------------------------------------
void ofxRibbon::clear(){

    for (int i = 0; i< parts.size(); i++){
        parts[i].clear();
    }
}

//--------------------------------------------------------------
void ofxRibbon::draw(){

    ofClear(255);
    for (int i = 0; i< parts.size(); i++){
        parts[i].draw();
    }
    
    if (bShowGui) {
        gui.draw();
    }
    
     //ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofxRibbon::mouseMoved(int x, int y ){
    
    bool useDirectMouse = true;
#ifdef ALREVES_USE_OSC
    if (Globals::oscIsMaster == false){
        useDirectMouse = false;
    }
#endif
    
    if (mode == RIBBON_MODE_FREE) {
        if (useDirectMouse) {
            for (int i = 0; i< parts.size(); i++){
                if (parts[i].mouseDriven){
                    parts[i].addPoint(x, y);;
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofxRibbon::nextMode(){
    
    if (mode == RIBBON_MODE_FREE){
        mode = RIBBON_MODE_HLINE;
        
        for (int i = 0; i< parts.size(); i++){
            parts[i].reset();
        }
        parts[0].mouseDriven = false;
        speedParam = 0;
        // easing
        startTime = ofGetElapsedTimef();
        endTime = startTime + 1;
        
        // draw 2 horizontal lines
        
    } else if (mode == RIBBON_MODE_HLINE) {
        mode = RIBBON_MODE_CIRCLE;
        for (int i = 0; i< parts.size(); i++){
            parts[i].reset();
        }
        // easing
        startTime = ofGetElapsedTimef();
        endTime = startTime + 1;
        speedParam = 1;
    } else if (mode == RIBBON_MODE_CIRCLE) {
        mode = RIBBON_MODE_SNAKE;
        for (int i = 0; i< parts.size(); i++){
            parts[i].reset();
        }
        // easing
        startTime = ofGetElapsedTimef();
        endTime = startTime + snakeSpeedParam;
        speedParam = 0;
        yOffset = 0;
    } else if (mode == RIBBON_MODE_SNAKE) {
        mode = RIBBON_MODE_HLINE_BOTTOM;
        for (int i = 0; i< parts.size(); i++){
            parts[i].reset();
        }
        parts[0].mouseDriven = false;
        speedParam = 0;
        // easing
        startTime = ofGetElapsedTimef();
        endTime = startTime + 1;
    } else if (mode == RIBBON_MODE_HLINE_BOTTOM) {
        mode = RIBBON_MODE_VLINES;
        for (int i = 0; i< parts.size(); i++){
            parts[i].reset();
        }
        parts[0].mouseDriven = false;
        speedParam = 0;
        // easing
        startTime = ofGetElapsedTimef();
        endTime = startTime + 10;
    }
}

//--------------------------------------------------------------
void ofxRibbon::keyPressed(int key){
    switch (key)
    {
        case ' ' : nextMode(); break;
        case 'c' : clear(); break;
        case 'r': setup(); break;
        case 'S' : gui.saveToFile(Globals::hostName + RIBBON_SETTINGS_FILE); break;
        case 'L' : gui.loadFromFile(Globals::hostName + RIBBON_SETTINGS_FILE); break;
        case 'g': bShowGui = !bShowGui; break;
        default:
            break;
    }
}
