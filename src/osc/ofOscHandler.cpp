//
//  ofOscHandler.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 02/06/2018.
//

#include "ofOscHandler.h"



//--------------------------------------------------------------
void ofOscHandler::setFileSafeHostNameToGlobals(){
    
    // Retrieve host name and store it in Globals
    
    FILE* stream = popen( "hostname", "r" );
    ostringstream output;
    
    while( !feof( stream ) && !ferror( stream ))
    {
        char buf[128];
        int bytesRead = fread( buf, 1, 128, stream );
        output.write( buf, bytesRead );
    }
    string hostname = ofSplitString(output.str(), ".")[0];
    hostname = ofSplitString(hostname, "\n")[0];
    
    // store value
    Globals::hostName = hostname;
    
    cout << "HostName: <" << Globals::hostName << ">" << endl;
}

//--------------------------------------------------------------
void ofOscHandler::setup(){
    
    if (bGuiLoaded == false){
        gui.setup("OSC control", Globals::hostName + OSC_SETTINGS_FILE);
        gui.add(isMasterParam.set("Master", false));
        gui.add(reconnectParam.set("reconnect",false));
        
        listenerGroup.setName("OSC Listener");
        listenerGroup.add(localServerPort.set("Port", 9000, 9000, 9010));
        gui.add(listenerGroup);
        
        senderGroup.setName("OSC Send");
        senderGroup.add(destServerNameParam.set("Destination", "127.0.0.1"));
        senderGroup.add(destServerPortParam.set("Port", 9000, 9000, 9010));
        gui.add(senderGroup);
        
        gui.loadFromFile(Globals::hostName + OSC_SETTINGS_FILE);
        
        bGuiLoaded = true;
    }
    
#ifdef ALREVES_USE_OSC
    
    // open an outgoing connection to HOST:PORT
    receiver.setup(localServerPort);
    cout <<  Globals::hostName + " is listening for osc messages on port " << localServerPort << "\n";
    
    Globals::oscIsMaster = isMasterParam;
    if (isMasterParam)
    {
        sender.setup(destServerNameParam, destServerPortParam);
        cout << "sending osc messages to " << destServerNameParam << ":" << destServerPortParam << "\n";
        
    }
#endif
    
}

//--------------------------------------------------------------
void ofOscHandler::update(const bool & debug){
    Globals::oscKeyPressed = 0;
    isDebugMode = debug;
#ifdef ALREVES_USE_OSC
    
    if (reconnectParam){
        // reset all connections
        setup();
        reconnectParam = false;
    }
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        
        // write to console on first message received to debug
        Globals::oscGotMessageFunc();
        
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        if (debug){
            numReceivedMessages = (numReceivedMessages + 1) % 30;
            if (numReceivedMessages == 0){
                receivedMessages = "";
            }
            string msg = getOscMsgAsString(m);
            receivedMessages += "\n" + msg;
        }
		
        string address = m.getAddress();
        // check for mouse moved message
        if(address == "/mouse/position"){
            // both the arguments are int32's
            Globals::oscMouseX = m.getArgAsInt32(0);
            Globals::oscMouseY = m.getArgAsInt32(1);
            //cout << "received mouse " << Globals::oscMouseX << endl;
        }        // check for mouse moved message
        else if(address == "/key"){
            // both the arguments are int32's
            Globals::oscKeyPressed = m.getArgAsInt32(0);
            cout << "received key " << Globals::oscKeyPressed  << endl;
        }
        else if(address.size() > 10 && address.substr(0,10) == "/OF/scene/"
                && Globals::oscIsMaster){
            // scene change command. Get 3rd part
            int on = m.getArgAsInt32(0);
            Globals::oscSceneIndex = on == 1 ? ofToInt(ofSplitString(address, "/")[3]) : -1;
          
            cout << "OSC scene changed to " << Globals::oscSceneIndex  << endl;
        }
        else if(address.size() > 16 && address.substr(0,16) == "/OF/slave/scene/"
                && !Globals::oscIsMaster){
            // scene change command. Get 3rd part
            int on = m.getArgAsInt32(0);
            Globals::oscSceneIndex = on == 1 ? ofToInt(ofSplitString(address, "/")[4]) : -1;
            
            cout << "OSC scene changed to " << Globals::oscSceneIndex  << endl;
        }
        else if(address == "/OF/key" && Globals::oscIsMaster){
            // key stroke from Ableton Live. The code is in the arg
            Globals::oscKeyPressed = m.getArgAsInt32(0);
            cout << "master key" << endl;
        }
        else if(address == "/OF/slave/key" && !Globals::oscIsMaster){
            // key stroke from Ableton Live. The code is in the arg
            Globals::oscKeyPressed = m.getArgAsInt32(0);
            cout << "slave key" << endl;
        }
    }
#endif
}

//--------------------------------------------------------------
void ofOscHandler::draw(){
    
    gui.draw();
    
    ofDrawBitmapStringHighlight(receivedMessages, 0, 0);
}

//--------------------------------------------------------------
void ofOscHandler::keyPressed(int key){
#ifdef ALREVES_USE_OSC
    if (Globals::oscIsMaster) {
        ofxOscMessage m;
        m.setAddress("/OF/slave/key");
        m.addIntArg(key);
        sender.sendMessage(m, false);
        if (isDebugMode){
            cout<<"sending key"<<endl;
        }
    }
#endif
}

//--------------------------------------------------------------
void ofOscHandler::keyReleased(int key){
#ifdef ALREVES_USE_OSC
    if (Globals::oscIsMaster) {
        ofxOscMessage m;
        m.setAddress("/OF/slave/key");
        m.addIntArg(0);
        sender.sendMessage(m, false);
        if (isDebugMode){
             cout<<"sending key released "<<endl;
        }
       
    }
#endif
}

//--------------------------------------------------------------
void ofOscHandler::mouseMoved(int x, int y ){
    
#ifdef ALREVES_USE_OSC
    if (Globals::oscIsMaster) {
        ofxOscMessage m;
        m.setAddress("/mouse/position");
        m.addIntArg(x);
        m.addIntArg(y);
        sender.sendMessage(m, false);
        if (isDebugMode){
            cout<<"sending mouse"<<endl;
        }
    }
#endif
}

//--------------------------------------------------------------
void ofOscHandler::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofOscHandler::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofOscHandler::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofOscHandler::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
string ofOscHandler::getOscMsgAsString(ofxOscMessage m){
    string msg_string;
    msg_string = m.getAddress();
    msg_string += ":";
    for(int i = 0; i < m.getNumArgs(); i++){
        // get the argument type
        msg_string += " " + m.getArgTypeName(i);
        msg_string += ":";
        // display the argument - make sure we get the right type
        if(m.getArgType(i) == OFXOSC_TYPE_INT32){
            msg_string += ofToString(m.getArgAsInt32(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
            msg_string += ofToString(m.getArgAsFloat(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
            msg_string += m.getArgAsString(i);
        }
        else{
            msg_string += "unknown";
        }
    }
    return msg_string;
}

