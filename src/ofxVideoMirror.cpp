#include "ofxVideoMirror.h"

//--------------------------------------------------------------
void ofxVideoMirror::setup() {

	// Set capture dimensions of 320x240, a common video size.
	camWidth = 640;
	camHeight = 480;
	frameReady = false;

	if (running)
	{

		running = false;
		vidGrabber.setVerbose(true);
		//Select desired grabbing frame rate
		//vidGrabber.setDesiredFrameRate(30);
		vidGrabber.initGrabber(camWidth, camHeight);

		running = true;
	}

	if (!guiInitialized) {
		gui.setup();
		gui.add(translation.setup("Translation", ofVec3f(0, 0, 0), ofVec3f(-500, -500, -500), ofVec3f(500, 500, 500)));
		guiInitialized = true;
	}
}


//--------------------------------------------------------------
void ofxVideoMirror::update() {

	if (running) {

		vidGrabber.update();
		
		if (vidGrabber.isFrameNew()) {

			//Initialize output pixels
			ofPixels pixels = vidGrabber.getPixelsRef();
			//Get pixel arrays for grabber
			ofPixels &pixelsGrab = vidGrabber.getPixelsRef();

			//Scan pixels
			for (int y = 0; y<camHeight; y++) {
				for (int x = 0; x<camWidth/2; x++) {
					//Get grabber color
					ofColor colorGrab = pixelsGrab.getColor(x, y);
					
					pixels.setColor(x, y, colorGrab);
				}
				for (int x = camWidth / 2; x<camWidth; x++) {
					//Get grabber color
					ofColor colorGrab = pixelsGrab.getColor(camWidth-x, y);

					pixels.setColor(x, y, colorGrab);
				}
			}
			//Update image pixels
			image.setFromPixels(pixels);

			/*if (!image.isAllocated()) {
				image = ofImage(vidGrabber.getPixelsRef());
			}

			ofPixels img = vidGrabber.getPixelsRef();
			ofImage  crop2;
			ofImage  crop;
			ofImage mirror;
			img.cropTo(crop, 0, 0, camWidth / 2, camHeight);
			crop.getPixelsRef().pasteInto(image.getPixelsRef(), 0, 0);
			img.cropTo(crop2, camWidth / 2, 0, camWidth, 0);
			crop2.mirror(false, true);
			image.update();
			crop2.update();
			crop2.getPixelsRef().pasteInto(image.getPixelsRef(), camWidth / 2, 0);
			image.update();*/

			frameReady = true;
			
		}
	}
}

//--------------------------------------------------------------
void ofxVideoMirror::draw() {

	ofEnableAlphaBlending();
	ofPushMatrix();
	
	ofTranslate(translation);

	//ofScale(1, -1, 1);
	if (frameReady) {

		// draw the incoming, the grayscale, the bg and the thresholded difference
		ofSetHexColor(0xffffff);
		//vidGrabber.draw(0, -camHeight / 2.0);
		image.draw(0, 0, 0,ofGetScreenWidth(),ofGetScreenHeight());
	}

	ofDrawAxis(150);
	

	ofPopMatrix();
	ofDisableAlphaBlending();

	if (bShowGui) {
		gui.draw();
	}
}



//--------------------------------------------------------------
void ofxVideoMirror::keyPressed(int key) {
	switch (key)
	{
	case 'r': running = true;
		frameReady = false;
		setup();
		break;
	case 's':
		running = false;
		frameReady = false;
		vidGrabber.close();
		break;
	case 'h': bShowGui != bShowGui; break;
	}
}