//
//  ofMarilyn.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 26/05/2018.
//

#include "ofMarilyn.h"

bool bIsTopScreen = true;
float destinationMilieu = 0.7;

//--------------------------------------------------------------
void ofMarilyn::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    isExplosing = false;
    
    p.assign(5, ofBande());
    currentMode = 0;
    
    //------------------
    bIsTopScreen = false;
    
    screenHeight = ofGetHeight();
    screenWidth = ofGetWidth();
    
    //----------
    // Glitch
    glitchFbo.allocate(screenWidth, screenHeight);
    myGlitch.setup(&glitchFbo);
    
    if (!bGuiLoaded) {
        gui.setup("bandes", BANDE_SETTINGS_FILE); // most of the time you don't need a name but don't forget to call setup
        
        gui.add(color0Param.set("color0", ofColor::orange));
        gui.add(color1Param.set("color1", ofColor::blue));
        gui.add(color2Param.set("color2", ofColor::violet));
        gui.add(color3Param.set("color3", ofColor::pink));
        gui.add(color4Param.set("color4", ofColor::green));
        gui.add(blankStartParam.set("Blank start",true));
        bGuiLoaded = true;
        gui.loadFromFile(BANDE_SETTINGS_FILE);
    }
    
    setupTimeTriggers();
    resetBandes();
    
    bShowGui = false;
}

//--------------------------------------------------------------
void ofMarilyn::setupTimeTriggers(){
    
    timeTriggers.clear();
    ofResetElapsedTimeCounter();
    //updateTween(tweenFadein, easingsine,ofxTween::easeInOut,0,255);
    
#ifdef MOV_BANDES_DEBUG
    int curTime = 0;
    for(unsigned int i = 0; i <= MOV_state_glitch; i++){
        timeTriggers[i] = MOV_BANDES_DEF_NO_TIME;
    }
    //    timeTriggers[MOV_state_Blank]    = MOV_BANDES_DEF_NO_TIME;
    //    timeTriggers[MOV_state_AllDown]        = MOV_BANDES_DEF_NO_TIME;
    //    timeTriggers[MOV_state_ToutRecule]        = MOV_BANDES_DEF_NO_TIME;        // start chrono for song                            - 47 s
    //    timeTriggers[MOV_state_ToutRecule] =MOV_BANDES_DEF_NO_TIME;        // start chrono for song                            - 53.5 s
    //    timeTriggers[MOV_state_ToutAvanceMid] = MOV_BANDES_DEF_NO_TIME;        // start chrono for song                            - 1:02 s
    //    timeTriggers[MOV_state_DepartJardin] = MOV_BANDES_DEF_NO_TIME;        // start chrono for song                            - 1:10 s
    //    timeTriggers[MOV_state_Bleu] = MOV_BANDES_DEF_NO_TIME;
    //    timeTriggers[MOV_state_Rose] = MOV_BANDES_DEF_NO_TIME;
    //    timeTriggers[MOV_state_Violet] = MOV_BANDES_DEF_NO_TIME;
    //    timeTriggers[MOV_state_Orange] = MOV_BANDES_DEF_NO_TIME;
    //    timeTriggers[MOV_state_Vert] = MOV_BANDES_DEF_NO_TIME;
#else
    // Times are all absolute times
    timeTriggers[MOV_state_Blank]    = 8;
    timeTriggers[MOV_state_AllDown]        = 13;
    timeTriggers[MOV_state_ToutRecule]        = 13.5;        // start chrono for song                            - 47 s
    timeTriggers[MOV_state_ToutRecule] =MOV_BANDES_DEF_NO_TIME;        // start chrono for song                            - 53.5 s
    timeTriggers[MOV_state_ToutAvanceMid] = MOV_BANDES_DEF_NO_TIME;        // start chrono for song                            - 1:02 s
    timeTriggers[MOV_state_DepartJardin] = MOV_BANDES_DEF_NO_TIME;        // start chrono for song                            - 1:10 s
    timeTriggers[MOV_state_Bleu] = MOV_BANDES_DEF_NO_TIME;
    timeTriggers[MOV_state_Rose] = MOV_BANDES_DEF_NO_TIME;
    timeTriggers[MOV_state_Violet] = MOV_BANDES_DEF_NO_TIME;
    timeTriggers[MOV_state_Orange] = MOV_BANDES_DEF_NO_TIME;
    timeTriggers[MOV_state_Vert] = MOV_BANDES_DEF_NO_TIME;
#endif
    
}

//--------------------------------------------------------------
void ofMarilyn::update(){
    
    float valA = getTweenAValue();
    float valB = getTweenBValue();
    switch (currentMode){
        case MOV_state_Blank:{
            for(unsigned int i = 0; i < p.size(); i++){
                p[i].screenWidth = screenWidth;
                p[i].screenHeight = screenHeight;
                
                p[i].update();
            }}
            break;
        case MOV_state_AllDown:
        {
            p[0].size.y = valA;
            p[1].size.y = valB;
            p[2].size.y = valA;
            p[3].size.y = valB;
            p[4].size.y = valA;
        }
            break;
        case MOV_state_ToutRecule:
        {
            for(unsigned int i = 0; i < p.size(); i++){
                p[i].size.y = valA;
                
                p[i].update();
            }
        }
            break;
        case MOV_state_ToutAvanceMid:
        {
            float posA = getTweenAValue();
            for(unsigned int i = 0; i < p.size(); i++){
                p[i].size.y = valA;
                
                p[i].update();
            }
        }
            break;
        case MOV_state_DepartJardin:
        {
            for(unsigned int i = 0; i < p.size(); i++){
                p[i].size.y = valA;
                p[i].pos.x -= valB;
                
                p[i].update();
            }
        }
            break;
        case MOV_state_Bleu:
        {
            p[0].size.y = 0;
            p[1].size.y = valA;
            p[2].size.y = 0;
            p[3].size.y = 0;
            p[4].size.y = 0;
        }
            break;
        case MOV_state_Rose:
        { // B
            p[0].size.y = 0;
            p[1].size.y = valA;
            p[2].size.y = 0;
            p[3].size.y = valB;
            p[4].size.y = 0;
        }
            break;
        case MOV_state_Violet:
        { // A
            p[0].size.y = 0;
            p[1].size.y = screenHeight*destinationMilieu;
            p[2].size.y = valA;
            p[3].size.y = valB;
            p[4].size.y = 0;
        }
            break;
        case MOV_state_Orange:
        { // B
            p[0].size.y = valB;
            p[1].size.y = screenHeight*destinationMilieu;
            p[2].size.y = valA;
            p[3].size.y = screenHeight*destinationMilieu;
            p[4].size.y = 0;
        }
            break;
        case MOV_state_Vert:
        { // A
            p[0].size.y = valB;
            p[1].size.y = screenHeight*destinationMilieu;
            p[2].size.y = screenHeight*destinationMilieu;
            p[3].size.y = screenHeight*destinationMilieu;
            p[4].size.y = valA;
        }
            break;
        case MOV_state_i:
        { // B
            p[0].size.y = valB;
            p[2].size.y = valB;
            p[4].size.y = valB;
        }
            break;
        case MOV_state_ii:
        { // B
            p[0].size.y = valB;
            p[2].size.y = valB;
            p[4].size.y = valB;
            
            p[1].size.y = valA;
            p[3].size.y = valA;
        }
            break;
        case MOV_state_iii:
        { // B
            p[0].size.y = valB;
            p[2].size.y = valB;
            p[4].size.y = valB;
            
            p[1].size.y = valA;
            p[3].size.y = valA;
        }
            break;
        case MOV_state_ivacour:
        case MOV_state_ivccour:
        case MOV_state_ivbjardin:
        case MOV_state_ivdmilieu:
        { // A
            for(unsigned int i = 0; i < p.size(); i++){
                p[i].size.y = screenWidth/2;
                p[i].pos.x = p[i].initialPos.x + valA;
                
                p[i].update();
            }
        }
            break;
        case MOV_state_1col2:{
            custom_bandeFullScreen(2);
        }    break;
        case MOV_state_1col4:{
            custom_bandeFullScreen(4);
        }    break;
        case MOV_state_1col0:{
            custom_bandeFullScreen(0);
        }    break;
        case MOV_state_1col3:{
            custom_bandeFullScreen(3);
        }    break;
        case MOV_state_1col1:
            custom_bandeFullScreen(1);
            break;
        case MOV_state_allCol:{
            // col2 grows in size until full screen
            for(unsigned int i = 0; i < p.size(); i++){
                p[i].size.y = (screenWidth/2)*(1+(1-valB));
                p[i].update();
                
                p[i].color.a = ofMap(valA,0,1,0,255);
            }
            p[1].color.a = 255;
            p[1].pos.x = ofMap(valB,0,1,0,p[2].initialPos.x);
            p[1].sizeFactor = ofPoint(1+(valB*screenWidth),1+(valB*screenWidth));
        } break;
        default:
            break;
    }
    
    
    
    if (ofGetElapsedTimef() > timeTriggers[currentMode])
    {
        nextMode("time trigger");
    }
}


//--------------------------------------------------------------
void ofMarilyn::custom_bandeFullScreen(const int bandeIndex){
    float valA = getTweenAValue();
    float valB = getTweenBValue();
    resetBandes();
    // col2 grows in size until full screen
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].size.y = (screenWidth/2)*(1+valB);
        p[i].update();
        
        p[i].color.a = ofMap(valA,0,1,255,0);
    }
    p[bandeIndex].color.a = 255;
    p[bandeIndex].pos.x = ofMap(valB,0,1,p[2].initialPos.x,0);
    p[bandeIndex].sizeFactor = ofPoint(1+(valB*screenWidth),1+(valB*screenWidth));
}

//--------------------------------------------------------------
void ofMarilyn::nextMode(std::string reason){
    currentMode+=1; // next mode
    
    if (currentMode == timeTriggers.size()) {
        cout << "end of triggers " << timeTriggers.size() << endl;
        setup();
    }
    
    cout << " at time : "
    << ofToString(floorf(ofGetElapsedTimef()/60.0))
    << ":" + ofToString(ofGetElapsedTimef() - 60*floorf(ofGetElapsedTimef()/60.0))
    << " -> new mode: " + ofToString(currentMode)
    << ", reason: " << reason
    << endl;
    
    float now = ofGetElapsedTimef();
    
    switch(currentMode) {
        case MOV_state_Blank:
        {
            ofResetElapsedTimeCounter();
            for(unsigned int i = 0; i < p.size(); i++){
                p[i].sizeFactor = ofPoint(0, 0);
                p[i].update();
            }
        }
            break;
        case MOV_state_AllDown:
        {
            for(unsigned int i = 0; i < p.size(); i++){
                p[i].sizeFactor = ofPoint(1, 1);
                p[i].update();
            }
            updateTweenA(EASING_CUBIC, 0,screenHeight,5);
            updateTweenB(EASING_CUBIC, 0,screenHeight,11);
        }
            break;
        case MOV_state_ToutRecule:
        {
            updateTweenA(EASING_EXPO_EASEOUT, screenHeight, 0, 0.5);
        }
            break;
        case MOV_state_ToutAvanceMid:
            updateTweenA(EASING_SINE, 0, screenHeight/2, 16);
            break;
        case MOV_state_DepartJardin:
            // offset x position (ie: pos -= tween value;
            updateTweenB(EASING_SINE, 0, screenWidth, 13);
            break;
        case MOV_state_Bleu:{
            float bandWidth = screenWidth / p.size();
            for(unsigned int i = 0; i < p.size(); i++){
                p[i].size.y = screenHeight/2;
                p[i].pos = ofPoint(bandWidth * i , 0);
                p[i].update();
            }
            updateTweenA(EASING_SINE_EASEOUT, 0, screenHeight*destinationMilieu, 7);
            //updateTweenB(EASING_SINE, 0, screenWidth, 0.5);
        }
            break;
        case MOV_state_Rose:
            updateTweenB(EASING_SINE_EASEOUT, 0, screenHeight*destinationMilieu, 7);
            break;
        case MOV_state_Violet:
            updateTweenA(EASING_SINE_EASEOUT, 0, screenHeight*destinationMilieu, 7);
            break;
        case MOV_state_Orange:
            updateTweenB(EASING_SINE_EASEOUT, 0, screenHeight*destinationMilieu, 7);
            break;
        case MOV_state_Vert:
            updateTweenA(EASING_SINE_EASEOUT, 0, screenHeight*destinationMilieu, 7);
            break;
        case MOV_state_i:
            updateTweenB(EASING_SINE_EASEOUT, getTweenBValue(), screenHeight, 6);
            break;
        case MOV_state_ii:{
            updateTweenA(EASING_SINE_EASEOUT, screenHeight*destinationMilieu, screenHeight, 4);
            updateTweenB(EASING_SINE_EASEOUT, screenHeight, screenHeight/2, 4);
        }
            break;
        case MOV_state_iii:{
            updateTweenA(EASING_SINE_EASEOUT, getTweenAValue(), screenHeight/2, 4);
            updateTweenB(EASING_SINE_EASEOUT, getTweenBValue(), screenHeight/2, 4);
        }
            break;
        case MOV_state_ivacour:{
            // offset x position (ie: pos += tween value;
            updateTweenA(EASING_SINE_EASEOUT, 0, screenWidth, 5);
        }
            break;
        case MOV_state_ivbjardin:
            updateTweenA(EASING_SINE_EASEOUT, screenWidth, -screenWidth, 2);
            break;
        case MOV_state_ivccour:
            updateTweenA(EASING_SINE_EASEOUT, -screenWidth, screenWidth, 2);
            break;
        case MOV_state_ivdmilieu:
            updateTweenA(EASING_SINE_EASEOUT, screenWidth, 0, 2);
            break;
        case MOV_state_vmelange:
            // TODO
            break;
        case MOV_state_1col2:
        case MOV_state_1col4:
        case MOV_state_1col0:
        case MOV_state_1col3:
        case MOV_state_1col1:{
            updateTweenB(EASING_SINE_EASEOUT, 0, 1, 0.5);
            updateTweenA(EASING_SINE_EASEOUT, 0, 1, 0.5);
        } break;
        case MOV_state_allCol:{
            updateTweenB(EASING_SINE_EASEOUT, 1, 0, 0.5);
            updateTweenA(EASING_SINE_EASEOUT, 0, 1, 0.5);
        } break;
        default:{
            tweenDuration = (timeTriggers[currentMode] - timeTriggers[currentMode-1])*1000;
            updateTweenA(EASING_EXPO_EASEOUT, 0,PI*0.9,tweenDuration);
        } break;
            
            
    }
}


//--------ç------------------------------------------------------
void ofMarilyn::resetBandes(){
    
    float bandWidth = screenWidth / p.size();
    p[0].color = color0Param;
    p[1].color = color1Param;
    p[2].color = color2Param;
    p[3].color = color3Param;
    p[4].color = color4Param;
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setMode(BANDE_MODE_IDLE);
        p[i].screenWidth = screenWidth;
        p[i].screenHeight = screenHeight;
        
        p[i].pos = ofPoint(bandWidth * i , 0);
        p[i].initialPos = ofPoint(bandWidth * i , 0);
        p[i].size = ofPoint(bandWidth, bIsTopScreen ? 0 : screenHeight);
        p[i].sizeFactor =  ofPoint(1, 1);
        
    }
}

//--------------------------------------------------------------
void ofMarilyn::draw(){
    ofClear(255);
    ofSetColor(255);
    glitchFbo.begin();
    // ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    ofClear(255);
    
    //    cam.begin();
    //    ofPushMatrix();
    //    ofTranslate(-screenWidth/2, -screenHeight/2);
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].draw();
    }
    glitchFbo.end();
    //
    //    /* draw normal view */
    //    ofSetColor(255);
    //    glitchFbo.draw(0, 0);
    
    /* Apply effects */
    myGlitch.generateFx();
    
    /* draw effected view */
    ofSetColor(255);
    glitchFbo.draw(0, 0);
    
    //    ofPopMatrix();
    //    cam.end();
    
    if (bShowGui) {
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofMarilyn::keyPressed(int key){
    if( key == 's'){
        bIsTopScreen = !bIsTopScreen;
    }
    if (key == ' '){
        // force change mode
        // update all time triggers to match the time offset
        if (timeTriggers[currentMode] != MOV_BANDES_DEF_NO_TIME){
            float deltaTime = timeTriggers[currentMode] - ofGetElapsedTimef();
            for (int i = currentMode; i < timeTriggers.size(); i++) {
                timeTriggers[i] -= deltaTime;
            }
            
            cout << "adjusted time triggers: t = t - " << ofToString(deltaTime) << endl;
        }
        nextMode("keyPressed");
    }
    
    if( key == 'r' ){
        setup();
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
        gui.saveToFile(BANDE_SETTINGS_FILE);
    }
    if (key == 'L') {
        gui.loadFromFile(BANDE_SETTINGS_FILE);
    }
    
    if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE    , true);
    if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW            , true);
    if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER            , true);
    if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER        , true);
    if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST            , true);
    if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE        , true);
    if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE            , true);
    if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN        , true);
    if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL            , true);
    if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT            , true);
}


//--------------------------------------------------------------
void ofMarilyn::updateTweenA(int easing, float _from, float _to, float _duration ){
    cout << "changed tween A" << endl;
    noiseEasing = easing;
    noiseFrom = _from;
    noiseTo = _to;
    noiseInitTime = ofGetElapsedTimef();
    noiseEndTime = noiseInitTime + _duration;
}
float ofMarilyn::getTweenAValue() {
    float val = 0;
    float now = ofGetElapsedTimef();
    if(now> noiseEndTime)
        return noiseTo;
    
    switch (noiseEasing) {
        case EASING_CUBIC:
            val = ofxeasing::map(now, noiseInitTime, noiseEndTime, noiseFrom, noiseTo, &ofxeasing::cubic::easeInOut);
            break;
        case EASING_CUBIC_EASEOUT:
            val = ofxeasing::map(now, noiseInitTime, noiseEndTime, noiseFrom, noiseTo, &ofxeasing::cubic::easeOut);
            break;
        case EASING_SINE:
            val = ofxeasing::map(now, noiseInitTime, noiseEndTime, noiseFrom, noiseTo, &ofxeasing::sine::easeInOut);
            break;
        case EASING_SINE_EASEOUT:
            val = ofxeasing::map(now, noiseInitTime, noiseEndTime, noiseFrom, noiseTo, &ofxeasing::sine::easeOut);
            break;
        case EASING_LINEAR:
            val = ofxeasing::map(now, noiseInitTime, noiseEndTime, noiseFrom, noiseTo, &ofxeasing::linear::easeInOut);
            break;
        case EASING_LINEAR_EASEOUT:
            val = ofxeasing::map(now, noiseInitTime, noiseEndTime, noiseFrom, noiseTo, &ofxeasing::linear::easeOut);
            break;
        case EASING_EXPO:
            val = ofxeasing::map(now, noiseInitTime, noiseEndTime, noiseFrom, noiseTo, &ofxeasing::exp::easeInOut);
            break;
        case EASING_EXPO_EASEOUT:
            val = ofxeasing::map(now, noiseInitTime, noiseEndTime, noiseFrom, noiseTo, &ofxeasing::exp::easeOut);
            break;
            
            
    }
    
    return val;
}
float ofMarilyn::getTweenBValue() {
    float val = 0;
    float now = ofGetElapsedTimef();
    if(now> fadeInEndTime)
        return fadeInTo;
    
    switch (fadeEasing) {
        case EASING_CUBIC:
            val = ofxeasing::map(now, fadeinInitTime, fadeInEndTime, fadeInFrom, fadeInTo, &ofxeasing::cubic::easeInOut);
            break;
        case EASING_CUBIC_EASEOUT:
            val = ofxeasing::map(now, fadeinInitTime, fadeInEndTime, fadeInFrom, fadeInTo, &ofxeasing::cubic::easeOut);
            break;
        case EASING_SINE:
            val = ofxeasing::map(now, fadeinInitTime, fadeInEndTime, fadeInFrom, fadeInTo, &ofxeasing::sine::easeInOut);
            break;
        case EASING_SINE_EASEOUT:
            val = ofxeasing::map(now, fadeinInitTime, fadeInEndTime, fadeInFrom, fadeInTo, &ofxeasing::sine::easeOut);
            break;
        case EASING_LINEAR:
            val = ofxeasing::map(now, fadeinInitTime, fadeInEndTime, fadeInFrom, fadeInTo, &ofxeasing::linear::easeInOut);
            break;
        case EASING_LINEAR_EASEOUT:
            val = ofxeasing::map(now, fadeinInitTime, fadeInEndTime, fadeInFrom, fadeInTo, &ofxeasing::linear::easeOut);
            break;
        case EASING_EXPO:
            val = ofxeasing::map(now, fadeinInitTime, fadeInEndTime, fadeInFrom, fadeInTo, &ofxeasing::exp::easeInOut);
            break;
        case EASING_EXPO_EASEOUT:
            val = ofxeasing::map(now, fadeinInitTime, fadeInEndTime, fadeInFrom, fadeInTo, &ofxeasing::exp::easeOut);
            break;
            
            
    }
    
    return val;
}
void ofMarilyn::updateTweenB(int easing, float _from, float _to, float _duration ){
    cout << "changed tween B" << endl;
    fadeEasing = easing;
    fadeInFrom = _from;
    fadeInTo = _to;
    fadeinInitTime = ofGetElapsedTimef();
    fadeInEndTime = fadeinInitTime + _duration;
}


//--------------------------------------------------------------
void ofMarilyn::keyReleased(int key){
    if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE    , false);
    if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW            , false);
    if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER            , false);
    if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER        , false);
    if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST            , false);
    if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE        , false);
    if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE            , false);
    if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN        , false);
    if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL            , false);
    if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT            , false);
}

//--------------------------------------------------------------
void ofMarilyn::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofMarilyn::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofMarilyn::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofMarilyn::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofMarilyn::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofMarilyn::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofMarilyn::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofMarilyn::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofMarilyn::dragEvent(ofDragInfo dragInfo){
    
}

