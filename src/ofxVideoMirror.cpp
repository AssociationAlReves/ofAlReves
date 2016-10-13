#include "ofxVideoMirror.h"

//--------------------------------------------------------------
void ofxVideoMirror::setup() {

	// Set capture dimensions of 320x240, a common video size.
	camWidth = 640;
	camHeight = 480;
	frameReady = false;
	curMode = enMirror3Sides;
	setMode(curMode);

	if (running)
	{
		running = false;
		vidGrabber.setVerbose(true);
		//Select desired grabbing frame rate
		//vidGrabber.setDesiredFrameRate(30);
		vidGrabber.setDeviceID(0);
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

			switch (curMode) {
			case enMirrorNormal:

				//Scan pixels
				for (int y = 0; y < camHeight; y++) {
					for (int x = 0; x < camWidth * curMirrorFactor; x++) {
						//Get grabber color
						ofColor colorGrab = pixelsGrab.getColor(x, y);

						pixels.setColor(x, y, colorGrab);
					}
					for (int x = camWidth * curMirrorFactor; x < camWidth; x++) {
						//Get grabber color
						ofColor colorGrab = pixelsGrab.getColor(camWidth - x, y);

						pixels.setColor(x, y, colorGrab);
					}
				}
				break;

			case enMirror3Sides:

				//Scan pixels
				for (int y = 0; y < camHeight; y++) {
					for (int x = camWidth * curMirrorFactor; x < camWidth * 2 * curMirrorFactor; x++) {
						//Get grabber color
						ofColor colorGrab = pixelsGrab.getColor(x, y);

						pixels.setColor(x, y, colorGrab);
					}
					for (int x = 0; x < camWidth * curMirrorFactor; x++) {
						//Get grabber color
						int xIn = ofClamp(camWidth * 2 * curMirrorFactor - x, 0, camWidth - 1);
						int xOut = ofClamp(x + camWidth * 2 * curMirrorFactor, 0, camWidth - 1);

						ofColor colorGrab = pixelsGrab.getColor(xIn, y);

						pixels.setColor(x, y, colorGrab);
						pixels.setColor(xOut, y, colorGrab);
					}

				}
				break;
			case enMirrorMiddle:
				//Scan pixels
				for (int y = 0; y < camHeight; y++) {
					int xMin = ofClamp(camWidth * curMirrorFactor, 0, camWidth);
					int xMax = ofClamp(xMin + camWidth * 2 * curMirrorFactor, 0, camWidth);
					for (int x = xMin; x <= xMax; x++) {
						//Get grabber color
						ofColor colorGrab = pixelsGrab.getColor(x, y);

						int xOut1 = ofClamp(x - xMin, 0, camWidth - 1);
						int xOut2 = ofClamp(camWidth - (xOut1), 0, camWidth - 1);

						pixels.setColor(xOut1, y, colorGrab);
						pixels.setColor(xOut2, y, colorGrab);
					}
				}
				break;
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


	//ofTranslate(translation);

	ofDrawAxis(500);
	ofPushMatrix();
	ofTranslate(0, ofGetScreenHeight());
	ofScale(1, -1, 1);
	if (frameReady) {

		// draw the incoming, the grayscale, the bg and the thresholded difference
		ofSetHexColor(0xffffff);
		//vidGrabber.draw(0, -camHeight / 2.0);
		image.draw(0, 0, 0, ofGetScreenWidth(), ofGetScreenHeight());
	}
	ofPopMatrix();



	ofDisableAlphaBlending();

	if (bShowGui) {
		gui.draw();


		stringstream ss;
		ss << "r: start video" << endl;
		ss << "s: stop video" << endl;
		ss << "h: show/hide help" << endl;
		ss << "q/d: change mirror width (current: " << curMirrorFactor << ")" << endl;
		ss << "m: change mirror mode (current: " << curMode << ")" << endl;
		ofDrawBitmapStringHighlight(ss.str(), 10, 10);
	}
}

void ofxVideoMirror::setMode(int mode) {
	switch (mode) {
	case enMirrorNormal:
		curMirrorFactor = 0.5f;
		break;
	case enMirror3Sides:
		curMirrorFactor = 1. / 3.0f;
		break;
	case enMirrorMiddle:
		curMirrorFactor = 0.25f;
		break;
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
	case 'm':
		curMode = (curMode + 1) % 3;
		setMode(curMode);		
		break;
	case 'q':
		curMirrorFactor = ofClamp(curMirrorFactor - 0.01, 0, 1);
		break;
	case 'd':
		curMirrorFactor = ofClamp(curMirrorFactor + 0.01, 0, 1);
		break;
	case 'h': bShowGui != bShowGui;
		break;
	}
}