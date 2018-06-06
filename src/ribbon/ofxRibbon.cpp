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
        gui.loadFromFile(Globals::hostName + RIBBON_SETTINGS_FILE);
        
        bGuiLoaded = true;
    }
    
    parts.assign(11, ofxRibbonPart());
    parts[0].mouseDriven = true;

}

//--------------------------------------------------------------
void ofxRibbon::update(){

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
}



//--------------------------------------------------------------
void ofxRibbon::mouseMoved(int x, int y ){
    for (int i = 0; i< parts.size(); i++){
        if (parts[i].mouseDriven){
            parts[i].addPoint(x, y);;
        }
    }
}



//--------------------------------------------------------------
void ofxRibbon::keyPressed(int key){
	switch (key)
	{
	case ' ' : clear(); break;
	case 'r': setup(); break;
        case 'S' : gui.saveToFile(Globals::hostName + RIBBON_SETTINGS_FILE); break;
        case 'L' : gui.loadFromFile(Globals::hostName + RIBBON_SETTINGS_FILE); break;
        case 'g': bShowGui = !bShowGui; break;
	default:
		break;
	}
}
