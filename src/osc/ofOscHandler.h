//
//  ofOscHandler.h
//  ofAlReves
//
//  Created by Xavier Fischer on 02/06/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "globals.h"
#include "ofxOsc.h"

#define OSC_SETTINGS_FILE  "osc_settings.xml"

/*
 
 +----------------+                         +----------------+
 |    Ableton     |                         |  OF APP #1     |  // responds only to OF/scene or OF/key
 |     LIVE       |                         |  MASTER        |
 +----------------+                         +----------------+
 | OSC SEND 9000  +---------------------+---> OSC LISTEN 9000|
 +----------------+  Scene change       |   +----------------+
                     Audio+fixed bangs  |   | OSC SEND 9000  +--+
                     (changes           |   +----------------+  |
                      within scene)     |                       | Mouse e^ents
                                        |                       | Key strokes
                     with MIDI to OSC   |   +----------------+  |
                                        |   |  OF APP #2     |  |
                                        |   |  SLAVE         |  |   // responds only to OF/slave/scene
                                        |   +----------------+  |      or OF/slave/key
                                        +---> OSC LISTEN 9000<--+
                                            +----------------+

 */
class ofOscHandler {
    
private:
    void setFileSafeHostNameToGlobals();
    string getOscMsgAsString(ofxOscMessage m);
    string receivedMessages;
    int numReceivedMessages;
    
    bool isDebugMode;
    
public:
    
    ofOscHandler() {
        setFileSafeHostNameToGlobals();
        bGuiLoaded = false;
        receivedMessages = "";
        numReceivedMessages = 0;
        isDebugMode = false;
    }
    
    void setup();
    void update(const bool & debug);
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    // gui params
    ofxPanel gui;
    
    ofParameter<bool> isMasterParam;
    ofParameter<bool> reconnectParam;
    
    ofParameterGroup senderGroup;
    ofParameter<string> destServerNameParam;
    ofParameter<int> destServerPortParam;
    
    ofParameterGroup listenerGroup;
    ofParameter<int> localServerPort;
    bool bGuiLoaded;
    
    // OSC
    ofxOscSender sender;
    ofxOscReceiver receiver;
    
};
