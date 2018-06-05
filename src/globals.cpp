//
//  globals.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 11/04/2018.
//
#include "globals.h"
#include "ofMain.h"



int Globals::oscMouseX = 0;
int Globals::oscMouseY = 0;
int Globals::screenWidth = 1280;
int Globals::screenHeight = 800;

int Globals::oscKeyPressed = 0;

bool Globals::oscIsMaster = false;
bool Globals::oscGotMessage = false;

int Globals::oscSceneIndex = 0;



void Globals::oscGotMessageFunc(){
	if (oscGotMessage == false){
		oscGotMessage = true;
		cout << "OSC GOT MESSAGE" << endl;
	}
}
string Globals::hostName = "";
