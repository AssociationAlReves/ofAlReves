#include "ofxKinecticon.h"


//--------------------------------------------------------------
void ofxKinecticon::setup(){

#ifdef USE_KINECT
	kinectEnabled = true;
	bKinectUseBg = true;
	bKinectSetup = true;
	bKinectAltColor = true;
	kinectMeshCurColor = 0;

	kinectMeshColors.push_back(ofColor::fromHex(0x0000FF));
	kinectMeshColors.push_back(ofColor::fromHex(0x00FF00));
	kinectMeshColors.push_back(ofColor::fromHex(0xFF0000));
	kinectMeshColors.push_back(ofColor::fromHex(0x00FFFF));
	kinectMeshColors.push_back(ofColor::fromHex(0xFFFF00));
	kinectMeshColors.push_back(ofColor::fromHex(0xFF00FF));

	// enable depth->video image calibration
	kinect.setRegistration(true);

	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)

	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #

	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}

#ifdef USE_TWO_KINECTS
	kinect2.init();
	kinect2.open();
#endif

	colorImg.allocate(kinect.width, kinect.height);
	liveImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	grayBgImage.allocate(kinect.width, kinect.height);
	grayDiff.allocate(kinect.width, kinect.height);

	nearThreshold = 255;
	farThreshold = 0;
	threshold = 80;
	bLearnBackground = false;
	numFramesBg = 0;

	//
	//ofSetFrameRate(60);

	// zero the tilt on startup
	angle = 17;
	kinect.setCameraTiltAngle(angle);

	// start from the front
	bDrawPointCloud = true;


	bShowHelp = false;
	gui = new ofxUISuperCanvas("Vasarely", OFX_UI_FONT_SMALL);        //Creates a canvas at (0,0) using the default width

	gui->addLabel("kinect");
	gui->addIntSlider("nearThresh", 0, 255, &nearThreshold);
	gui->addIntSlider("farThresh", 0, 255, &farThreshold);
	gui->addIntSlider("diffThresh", 0, 255, &threshold);
	gui->addLabelToggle("Use background (b)", &bKinectUseBg);
	gui->addLabelToggle("Alt Color (c)", &bKinectAltColor);
	gui->setVisible(false);

#endif

}

//--------------------------------------------------------------
void ofxKinecticon::update(){


#ifdef USE_KINECT
	kinect.update();

	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {

		// load grayscale depth image and color image from the kinect source
		liveImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		colorImg.setFromPixels(kinect.getPixels(),kinect.width,kinect.height);


		bool bThreshWithOpenCV = false;
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds

		if(bThreshWithOpenCV) {
			grayThreshNear = liveImage;
			grayThreshFar = liveImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), liveImage.getCvImage(), NULL);
		} else {

			// or we do it ourselves - show people how they can work with the pixels
			unsigned char * pix = liveImage.getPixels();

			int numPixels = liveImage.getWidth() * liveImage.getHeight();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					pix[i] = pix[i];
				} else {
					pix[i] = 0;
				}
			}
		}


		if (bLearnBackground) {

			if (numFramesBg == 0) {
				grayBgImage.allocate(kinect.width, kinect.height);
				cvCopy(liveImage.getCvImage(), grayBgImage.getCvImage());
			} else {
				//ofxCvGrayscaleImage clone = grayBgImage;
				cvOr(liveImage.getCvImage(), grayBgImage.getCvImage(),grayBgImage.getCvImage());
			}
			numFramesBg++;
			if (numFramesBg == VASA_KINECT_NUM_BG_FRAMES){
				bLearnBackground = false;
				numFramesBg=0;
				ofLogWarning("kinect") << "background OK";
				grayBgImage.flagImageChanged();

			}
		}


		// update the cv images
		liveImage.flagImageChanged();

		if (bKinectUseBg){
			if (bThreshWithOpenCV){
				grayDiff.absDiff(grayBgImage, liveImage);
				grayDiff.threshold(threshold);
			} else {
				// or we do it ourselves - show people how they can work with the pixels
				unsigned char * pix = liveImage.getPixels();
				unsigned char * bgpix = grayBgImage.getPixels();
				unsigned char * outpix = grayDiff.getPixels();

				int numPixels = liveImage.getWidth() * liveImage.getHeight();
				for(int i = 0; i < numPixels; i++) {
					outpix[i] = ofClamp(pix[i] - bgpix[i], 0,255);
				}
			}
		} else {
			grayDiff = liveImage;

		}

		grayDiff.threshold(threshold);
		grayDiff.flagImageChanged();

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		//params
		//        ofxCvGrayscaleImage&  input,
		//        int minArea,
		//        int maxArea,
		//        int nConsidered,
		//        bool bFindHoles,
		//        bool bUseApproximation
		contourFinder.findContours(grayDiff, 50*50, (kinect.width*kinect.height)/2, 10, false);

		grayDiff.flagImageChanged();


		if (bDrawPointCloud){

			// Create mesh for point cloud

			int w = kinect.width;
			int h = kinect.height;
			ofMesh mesh;
			mesh.setMode(OF_PRIMITIVE_POINTS);
			unsigned char * mask = grayDiff.getPixels();

			// retrieve only pixels from blobs
			// TODO take pixels only in found blobs rects

			for(int y = 0; y < h; y += VASA_KINECT_POINTCLOUD_STEP) {
				for(int x = 0; x < w; x += VASA_KINECT_POINTCLOUD_STEP) {
					if(mask[y * w + x] > 128) {
						if (!bKinectAltColor) {
							ofColor color = kinect.getColorAt(x, y);
							//                            float b = color.getBrightness();
							//                            if (b<200) color.setBrightness(200);
							//                            color.a = 200;
							mesh.addColor(color);
						}

						mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
					}
				}
			}

			if (kinectMeshes.size() == VASA_KINECT_FRAMECOUNT){
				kinectMeshes.erase(kinectMeshes.begin());
			}
			kinectMeshes.push_back(mesh);

		}
	}

#ifdef USE_TWO_KINECTS
	kinect2.update();
#endif
#endif

}

//--------------------------------------------------------------
void ofxKinecticon::draw() {

#ifdef USE_KINECT
	if (!kinectEnabled) return;

	ofSetColor(255, 255, 255);

	if (bKinectSetup){
		if(bDrawPointCloud) {
			drawPointCloud();
		} else {

			kinectMeshes.clear();

			// draw from the live kinect
			kinect.drawDepth(10, 10, 400, 300);
			kinect.draw(420, 10, 400, 300);

			liveImage.draw(10, 320, 400, 300);
			ofPushMatrix();
			ofTranslate(0,0,10);
			contourFinder.draw(10, 320, 400, 300);
			ofPopMatrix();

			grayBgImage.draw(420, 320,400,300);
			grayDiff.draw(420+400+10, 320,400,300);

#ifdef USE_TWO_KINECTS
			kinect2.draw(420, 320, 400, 300);
#endif
		}

		if (bShowHelp) {
			// draw instructions
			ofSetColor(255, 255, 255);
			stringstream reportStream;

			if(kinect.hasAccelControl()) {
				reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
					<< ofToString(kinect.getMksAccel().y, 2) << " / "
					<< ofToString(kinect.getMksAccel().z, 2) << endl;
                reportStream << "accel is: pitch: " << ofToString(kinect.getAccelPitch(), 2) << " / roll: "
                << ofToString(kinect.getAccelRoll(), 2) << endl;
			} else {
				reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
					<< "motor / led / accel controls are not currently supported" << endl << endl;
			}

			reportStream << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
				//<< "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
				<< "set near threshold " << nearThreshold << " (press: + -)" << endl
				<< "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
				<< ", fps: " << ofGetFrameRate() << endl
				<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;

			if(kinect.hasCamTiltControl()) {
				reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
					<< "press 1-5 & 0 to change the led mode" << endl;
			}

			ofDrawBitmapString(reportStream.str(), 20, 652);
		}
	}


#endif
}

//--------------------------------------------------------------
void ofxKinecticon::drawPointCloud(){

#ifdef USE_KINECT
	ofBackground(0);

	glPointSize(2);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, 1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	ofDisableDepthTest();
	kinectMeshCurColor = 0;
	if (kinectMeshes.size() == VASA_KINECT_FRAMECOUNT){
		for(int i = VASA_KINECT_FRAMECOUNT-1; i >= 0; i-=VASA_KINECT_FRAMEDELAY)
		{
			kinectMeshCurColor = (kinectMeshCurColor + 1) % kinectMeshColors.size();
			ofSetColor(kinectMeshColors[kinectMeshCurColor]);

			ofMesh curMesh = kinectMeshes[i];
			//            vector<ofFloatColor> meshColors = curMesh.getColorsPointer();
			//            for (int col = 0; col < meshColors.size();col++){
			//                meshColors[col].setHue(128);
			//            }

			curMesh.draw();
		}
	} else if (kinectMeshes.size()>0) {
		kinectMeshes[kinectMeshes.size()-1].draw();
	}
	ofDisableDepthTest();
	ofPopMatrix();

#endif
}

//--------------------------------------------------------------
void ofxKinecticon::keyPressed(int key){

#ifdef USE_KINECT
	if (kinectEnabled){
		switch (key) {
		case ' ':
			bLearnBackground = !bLearnBackground;
			break;

		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;

		case 'h': bShowHelp = !bShowHelp;
			break;

		case 'b':
			bKinectUseBg = !bKinectUseBg;
			break;

			        case '>':
			        case '.':
			            farThreshold ++;
			            if (farThreshold > 255) farThreshold = 255;
			            break;
			            
			        case '<':
			        case ',':
			            farThreshold --;
			            if (farThreshold < 0) farThreshold = 0;
			            break;
			            
			        case '+':
			        case '=':
			            nearThreshold ++;
			            if (nearThreshold > 255) nearThreshold = 255;
			            break;
			            
			        case '-':
			            nearThreshold --;
			            if (nearThreshold < 0) nearThreshold = 0;
			            break;

		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;

		case 'a':
			kinect.setCameraTiltAngle(18); // go back to prev tilt
			break;

		case 'c':
			bKinectAltColor = !bKinectAltColor;
			break;
                
        case 'C':
            kinect.setCameraTiltAngle(0); // zero the tilt on exit
            kinect.close();
            break;
        case 'O':
                kinect.open();
                kinect.setCameraTiltAngle(angle);
                break;

		case 'd':
			bKinectSetup = !bKinectSetup;
			break;
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;

		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;

		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;

		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;

		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;

		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
		case 'g' : gui->setVisible(!gui->isVisible());

		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;

		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
		}

	}
#endif
}

//--------------------------------------------------------------
void ofxKinecticon::exit() {
#ifdef USE_KINECT
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();

#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
#endif
}