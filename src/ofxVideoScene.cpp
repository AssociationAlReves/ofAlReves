#include "ofxVideoScene.h"



//--------------------------------------------------------------
void ofxVideoScene::setup(){
	player.loadMovie(fileName);
	player.play();
	player.setVolume(0);
    if (!autoLoop) {
        player.setLoopState(OF_LOOP_NONE);
    }
    aspectRatio = player.getWidth()/player.getHeight();
	if (autoPlay == false) {
		player.stop();
	}
}


//--------------------------------------------------------------
void ofxVideoScene::update(){
	player.update();
}

//--------------------------------------------------------------
void ofxVideoScene::draw(){
    
    ofEnableAlphaBlending();
	ofPushMatrix();

	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	if (horizontalFlip == false) {
		ofScale (1,-1,1);
	}
	ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
	ofBackground(0);
	if (fillScreen) {
		float pw = ofGetWidth();
		float ph = ofGetHeight();
		float xCenterOrigin = -(pw - ofGetWidth()) / 2;
		ofTranslate(xCenterOrigin, 0);
		player.draw(0, 0, pw, ph);
		//player.draw(0, 0, pw, ph);
	}
	else {
		float pw = ofGetWidth();
		float ph = player.getHeight() * ofGetWidth() / player.getWidth();
		float xCenterOrigin = -(pw - ofGetWidth()) / 2;
		float yCenterOrigin = -(ph - ofGetHeight()) / 2;
		ofTranslate(xCenterOrigin, yCenterOrigin);
		player.draw(0, 0, pw, ph);
		//player.draw(0, 0, pw, ph);
	}

	
	

	 
	ofPopMatrix();
    ofDisableAlphaBlending();
}



//--------------------------------------------------------------
void ofxVideoScene::keyPressed(int key){
	switch (key)
	{
	case ' ' : {
				if (player.isPlaying()) {
                    player.stop();
				} else { 
					player.play(); 
				}
			   }
			   break;
	case 'r': setup(); break;
	default:
		break;
	}
}