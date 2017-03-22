//
//  ofxVasaSquareField.cpp
//  ofVasarely
//
//  Created by Xavier Fischer on 05/05/2015.
//
//

#include "ofxVasaSquareField.h"

//--------------------------------------------------------------
void ofxVasaSquareField::setup(){

	bShowGui = false;
	distRange = 300;
	scaleFactor = 0.6;
	devAngle = 90;
	distRangeAuto = true;
	scaleFactorAuto = true;
	devAngleAuto = false;

	distRangeSpeed = 0.08;
	scaleFactorSpeed = 0.07;
	devAngleSpeed = 0.07;

	distRangeMIN = 20;
	scaleFactorMIN = 0.1;
	devAngleMIN = 0;

	distRangeMAX = PROJECTOR_RESOLUTION_Y;
	scaleFactorMAX = 1;
	devAngleMAX = 90;

	switch (mode) {
	case VASA_SQUARE_MODE_FULL_RND_ROTATION:
		setup(ofGetWidth(), ofGetHeight(), VASA_SQUARE_SIZE /* 50 */);
		break;
	case VASA_SQUARE_MODE_FULL_DIST_ROTATION:
		setup(ofGetWidth(), ofGetHeight(), VASA_SQUARE_SIZE /* 50 */);
		break;
	}

	if (bGuiInitialized == false) {

		gui.setup("Square Field", VASA_SQUARE_SETTINGS_FILE);        //Creates a canvas at (0,0) using the default width


		forceParams.setName("Force");
			forceParams.add(distRange.set("distance", 50, 0.1, 1000));
			forceParams.add(distRangeAuto.set("noise force", true));

			forceParams.add(distRangeMIN.set("range Min",20, 20, PROJECTOR_RESOLUTION_Y));
			forceParams.add(distRangeMAX.set("range Max",PROJECTOR_RESOLUTION_Y, 20, PROJECTOR_RESOLUTION_Y));
			forceParams.add(distRangeSpeed.set("speed", 5, 0.1, 10));
		gui.add(forceParams);
		
		scaleParams.setName("Scale");
			scaleParams.add(scaleFactor.set("factor", 1, 1, 0));
			scaleParams.add(scaleFactorAuto.set("noise", true));
			scaleParams.add(scaleFactorMIN.set("range Min",0, 0.1,1));
			scaleParams.add(scaleFactorMAX.set("range Max",0, 0.1,1));
			scaleParams.add(scaleFactorSpeed.set("speed", 0.1, 0.1, 10));
		gui.add(scaleParams);

		angleParams.setName("Angle");
			angleParams.add(devAngle.set("deviation", 0, 0, 360));
			angleParams.add(devAngleAuto.set("auto", true));
			angleParams.add(devAngleMIN.set("angle Min", 0, 0, 360));
			angleParams.add(devAngleMAX.set("angle Max", 360, 0, 360));		
			angleParams.add(devAngleSpeed.set("speed", 0.1, 0.1, 10));
		gui.add(angleParams);
		
		bGuiInitialized = true;
	}
}

//--------------------------------------------------------------
void ofxVasaSquareField::setup(int width, int height, int squareSize){

	lastActorX = 0;
	lastActoxY = 0;
	//ary[i*sizeY+j]
	sizeX = width/squareSize+1;
	sizeY = height/squareSize+1;
	squareCount = sizeX*sizeY;
	this->squareTotalSize = squareSize;
	this->padding = squareSize*VASA_SQUARE_PADDING_FACTOR;
	this->squareSize = squareSize-padding;

	rotMaxSpeed.clear();
	rotSpeed.clear();
	rotDecay.clear();
	actors.clear();

    if (mode == VASA_SQUARE_MODE_FULL_RND_ROTATION) {
        rotSpeed.reserve(squareCount);
        rotDecay.reserve(squareCount);
        
        
        
        
        for (int j = 0; j<sizeY;j++)
            for (int i = 0; i<sizeX;i++) {
                randIndexes.push_back(ofVec2f(i,j));
                float speed = 0; //ofRandom(1, VASA_SQUARE_SPEED);
                rotMaxSpeed.push_back(speed);
                rotSpeed.push_back(speed);
                rotDecay.push_back(ofRandom(VASA_SQUARE_DECAY_MIN, VASA_SQUARE_DECAY_MAX));
            }
        
        // random index for square random appearance
        ofSeedRandom(1234);
        ofRandomize(randIndexes);
        squareAutoComplete = false;
        if (squareSpawn)
        {
            squareIndex = 0;
        } else {
            squareIndex = squareCount;
        }
    }

}

//--------------------------------------------------------------
void ofxVasaSquareField::update(){

	if (mode == VASA_SQUARE_MODE_FULL_RND_ROTATION) {

		clearActors();
		addActor(mouseX, mouseY);

		for (int i = 0; i<sizeX;i++)
			for (int j = 0; j<sizeY;j++)
			{
				int idx = i*sizeY+j;
				rotSpeed[idx] *= rotDecay[idx];
			}

			//if (squareAutoComplete) {
			//	/*if (ofGetFrameNum() % 5 == 0) {
			//		squareIndex++;
			//	}*/
			//		squareIndex+=3;
			//	
			//}
	} else if (mode == VASA_SQUARE_MODE_FULL_DIST_ROTATION) {
		if (distRangeAuto) {
			distRange = ofNoise(ofGetElapsedTimef() * distRangeSpeed + 1.23)* distRangeMAX + distRangeMIN;
		}
		if (scaleFactorAuto) {
			scaleFactor = ofNoise(ofGetElapsedTimef() * scaleFactorSpeed +2.34) * scaleFactorMAX+ scaleFactorMIN;
		}
		if (devAngleAuto) {
			devAngle = ofNoise(ofGetElapsedTimef() * devAngleSpeed +3.45) * devAngleMAX + devAngleMIN;
		}

	}
	//gui->setVisible(ofxGetAppPtr()->isDebug());
}

//--------------------------------------------------------------
void ofxVasaSquareField::draw(){
	
	ofDisableAlphaBlending();
	ofClear(0);
	ofSetColor(0,alpha);ofFill();
	ofDrawRectangle(0,0,0,ofGetWidth(),ofGetHeight());
	ofSetColor(255,alpha);


    float speed = 0.25f; //1.f;

	ofPushMatrix();
	if (mode == VASA_SQUARE_MODE_FULL_RND_ROTATION) {
		ofTranslate(squareSize,squareSize);

		if (squareIndex < squareCount) {
			for (int idx = 0; idx < squareIndex; idx++)
			{
				int i = randIndexes[idx].x; 
				int j = randIndexes[idx].y;
				ofPushMatrix();
				ofTranslate(i*squareTotalSize-squareSize*(1.0*VASA_SQUARE_PADDING_FACTOR), j*squareTotalSize-squareSize*(1.0*VASA_SQUARE_PADDING_FACTOR));

				// noise rotation going to 0 when all squares are drawn
				ofRotateZ(ofSignedNoise( ofGetElapsedTimef() *2*speed + i*1.23 + j*2.34 )
					* ofMap(squareIndex, 0, squareCount, 180, 0));

				// noise scale  going to 1 when all squares are drawn
				float scaleXY = 1 + ofSignedNoise( ofGetElapsedTimef() *speed + i*7.89 + j*1.23) * ofMap(squareIndex, 0.75, squareCount, 15, 0);
				ofScale( scaleXY, scaleXY, 1);

				float xTrans = ofSignedNoise( ofGetElapsedTimef() *speed + (i+j)*7.89) * ofMap(squareIndex, 0, squareCount, 100, 0);
				float yTrans = ofSignedNoise( ofGetElapsedTimef() *speed + (i-j)*4.56) * ofMap(squareIndex, 0, squareCount, 100, 0);
				ofTranslate(xTrans, yTrans);

                //ofSetColor(255);
                
                ofDrawRectangle(-squareSize/2,-squareSize/2, squareSize,squareSize);
                
                /* squares outline
                 
                 ofSetColor(0);
                ofLine(-squareSize/2,-squareSize/2, squareSize/2,-squareSize/2);
                ofLine(squareSize/2,-squareSize/2, squareSize/2,squareSize/2);
                ofLine(squareSize/2,squareSize/2,-squareSize/2,squareSize/2);
                ofLine(-squareSize/2,squareSize/2,-squareSize/2,-squareSize/2);
                */
				ofPopMatrix();
			}
		}
		else
		{
			for (int i = 0; i<sizeX;i++)
				for (int j = 0; j<sizeY;j++)
				{
					ofPushMatrix();
					ofTranslate(i*squareTotalSize-squareSize*(1.0*VASA_SQUARE_PADDING_FACTOR), j*squareTotalSize-squareSize*(1.0*VASA_SQUARE_PADDING_FACTOR));
					ofRotateZ(rotSpeed[i*sizeY+j] * ofGetElapsedTimef());
					ofDrawRectangle(-squareSize/2,-squareSize/2, squareSize,squareSize);
					ofPopMatrix();

				}
		}
	} else if (mode == VASA_SQUARE_MODE_FULL_DIST_ROTATION) {

		ofTranslate(squareSize,squareSize);
		for (int i = 0; i<sizeX;i++)
			for (int j = 0; j<sizeY;j++)
			{
				float squareX = i*squareTotalSize-squareSize*(1.0*VASA_SQUARE_PADDING_FACTOR);
				float squareY = j*squareTotalSize-squareSize*(1.0*VASA_SQUARE_PADDING_FACTOR);
				ofPushMatrix();
				ofTranslate(squareX, squareY);

				// the closer, the greater is the force
				float force = distRange - ofClamp(ofDist(squareX, squareY, mouseX, mouseY),0,distRange);
				ofScale(ofMap(force,0,distRange, 1,scaleFactor),ofMap(force,0,distRange, 1,scaleFactor),1);
				ofRotateZ(ofMap(force,0,distRange, 0,devAngle));
				ofDrawRectangle(-squareSize/2,-squareSize/2, squareSize,squareSize);
				ofPopMatrix();

			}
	}

	ofPopMatrix();

	ofSetColor(0,0,0,alpha);
	ofDisableAlphaBlending();

	if (bShowGui) {
		gui.draw();
	}

}

//--------------------------------------------------------------
void ofxVasaSquareField::nextMode() {
	mode = (mode + 1) % 2;

	setup();
}

//--------------------------------------------------------------
void ofxVasaSquareField::addActor(int x, int y){
	x /= squareSize;
	y /= squareSize;

	if (x == lastActorX && y == lastActoxY) {
		return;
	}
	lastActorX = x;
	lastActoxY = y;
	//actors.push_back(ofVec2f(x,y));
	int radius = VASA_SQUARE_ACTOR_RADIUS;
	for (int j = ofClamp(y-radius, 0, sizeY); j<ofClamp(y+radius, 0, sizeY);j++)
		for (int i = ofClamp(x-radius, 0, sizeX); i<ofClamp(x+radius, 0, sizeX);i++) {
            if (ofDist(x,y,i,j) <= radius) { // distance check (better for having a round displacement shape instead of a squared shape)
                float speed = ofRandom(1, VASA_SQUARE_SPEED);
                rotMaxSpeed[i*sizeY+j] = speed;
                rotSpeed[i*sizeY+j] = speed;
                rotDecay[i*sizeY+j] = ofMap(ofNoise(i, j),0,1,VASA_SQUARE_DECAY_MIN,VASA_SQUARE_DECAY_MAX) ;// * ofRandom(VASA_SQUARE_DECAY_MIN, VASA_SQUARE_DECAY_MAX);
            }
        }
}

//--------------------------------------------------------------
void ofxVasaSquareField::clearActors(){
	actors.clear();
}

//--------------------------------------------------------------
void ofxVasaSquareField::hardReset(){
	for (int j = 0; j<sizeY;j++)
		for (int i = 0; i<sizeX;i++) {
			float speed = ofRandom(1, VASA_SQUARE_SPEED);
			rotMaxSpeed[i*sizeY+j] = speed;
			rotSpeed[i*sizeY+j] = speed;
			rotDecay[i*sizeY+j] = 0.8;
		}
}
//--------------------------------------------------------------
void ofxVasaSquareField::smoothReset(){
	for (int j = 0; j<sizeY;j++)
		for (int i = 0; i<sizeX;i++) {
			float speed = ofRandom(1, VASA_SQUARE_SPEED);
			rotMaxSpeed[i*sizeY+j] = speed;
			rotSpeed[i*sizeY+j] = speed;
			rotDecay[i*sizeY+j] = ofRandom(VASA_SQUARE_DECAY_MIN, VASA_SQUARE_DECAY_MAX);
		}
}


//--------------------------------------------------------------
void ofxVasaSquareField::keyPressed(int key){
	switch(key)
	{
	case 'c': hardReset();	break;
	case ' ': squareIndex += (squareAutoComplete ? 3 : 1);	break;
	case OF_KEY_RETURN: squareAutoComplete = !squareAutoComplete; break;
	case 'n': nextMode();	break;
	case 'w' : smoothReset(); break;
	case 'W' : hardReset(); break;
	case 'r' : setup(); break;
	case 'h':  bShowGui = !bShowGui; break;
	}
}



