#include "ofxCity.h"
#include "ofApp.h"

//--------------------------------------------------------------
void ofxCity::setup() {

	ofSeedRandom(123456);
	bUpdateParamsFromCode = true;

	// shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess(120);
	material.setSpecularColor(ofColor(255, 255, 255, 255));

	img.allocate(CITY_BLOCKS_COLS * 2 + 5, CITY_BLOCKS_ROWS, ofImageType::OF_IMAGE_COLOR);

	ofApp *app = (ofApp *)ofxGetAppPtr();
	app->transformer.cam.reset();
	//float farClip = app->transformer.cam.getFarClip();
	app->transformer.cam.setFarClip(1000000);

	mode = enCityIdle;
	curSpeed = 0;
	desiredSpeed = 0;
	curDistanceOffset = 0;
	curDistance = 0;
	rotationAngle = 0;
	translationCollapse = 0;
	buildings.clear();
	
	speedStartTime = 0;
	speedEndTime = 0;
	speedFrom = curSpeed;
	speedTo = desiredSpeed;

	// GUI
	roadParamsHash = 0;
	if (!bGuiLoaded) {
		gui.setup("cityPanel", Globals::hostName + CITY_SETTINGS_FILE); // most of the time you don't need a name but don't forget to call setup
		gui.add(bWireframe.set("Wireframe", false));
		gui.add(bTweenSpeed.set("Tween speed", true));
		gui.add(fov.set("FOV", 60, 0, 360));
		gui.add(debugFbo.set("Debug FBO", false));
		gui.add(camOrientation.set("camOrientation", app->transformer.cam.getOrientationEuler(), ofVec3f(-180, -180, -180), ofVec3f(180, 180, 180)));
		gui.add(camPosition.set("camPosition", app->transformer.cam.getPosition(), ofVec3f(-180, -180, -180), ofVec3f(180, 180, 180)));
		roadParams.setName("Road");

		roadParams.add(rotationAngle.set("rotationAngle", 0, -180, 180));
		roadParams.add(translationCollapse.set("translat", 0, 0, 100000));
		roadParams.add(roadOpacity.set("Road opacity", 0, 0, 255));
		roadParams.add(roadTexWidth.set("Road tex width", 100, 10, 1000));
		roadParams.add(roadTexHeight.set("Road tex height", 100, 10, 1000));
		roadParams.add(roadLineWidth.set("Road line width", 1.5, 1, 100));
		roadParams.add(roadLineHeight.set("Road line height", 50, 1, 100));
		roadParams.add(curSpeed.set("Velocity", 0, 0, 150));
		gui.add(roadParams);

		terrainParams.setName("terrain");
		terrainParams.add(terrainWidth.set("width", 200, 10, 1000));
		terrainParams.add(terrainHeight.set("height", 200, 10, 1000));
		terrainParams.add(segmentLength.set("segLength", 40, 1, 1000));
		terrainParams.add(terrainDrawX.set("draw X", true));
		terrainParams.add(terrainDrawY.set("draw Y", true));
		terrainParams.add(terrainZScale.set("Z scale", 0.2, 0, 50));
		terrainParams.add(terrainNoiseScale.set("noise scale", 1, 0, 10));
		terrainParams.add(terrainNoiseSeed.set("noise seed", 1, 0, 10));
		terrainParams.add(terrainNoiseAmp.set("noise amp", 50, 0, 100));
		gui.add(terrainParams);

		buildingParams.setName("Buildings");
		buildingParams.add(blockProbability.set("Proba", 0.75, 0, 1));
		buildingParams.add(autoGenerateBuildings.set("Auto generated", false));
		gui.add(buildingParams);

		lightParams.setName("Lights");
		lightParams.add(dirLightOrientation.set("Orientation (x,z,y angles)", ofVec3f(180, -42.2449, 117.551), ofVec3f(-180), ofVec3f(180)));
		lightParams.add(diffuseColor.set("diffuseColor", ofFloatColor(1, 0, 0, 1), ofFloatColor(0, 0, 0, 0), ofColor::white));
		lightParams.add(specularColor.set("specularColor", ofFloatColor(1, 0, 0, 1), ofFloatColor(0, 0, 0, 0), ofColor::white));
		lightParams.add(ambientColor.set("ambientColor", ofFloatColor(0), ofFloatColor(0, 0, 0, 0), ofColor::white));
		gui.add(lightParams);

		bGuiLoaded = true;


	}
	bShowGui = false;

	setupTextures();

	setupRoad();
	setupBlocks();

	ofSetLogLevel("ofxCity", OF_LOG_NOTICE);
	//ofSetLogLevel("ofxCity", OF_LOG_VERBOSE);

}

//--------------------------------------------------------------
void ofxCity::setupTextures() {
	
	fboRoad.allocate(roadTexWidth, roadTexHeight, GL_RGB32F /*GL_RGB32F_ARB*/);
	fboRoad.begin();

	ofBackground(ofColor::gray);
	ofSetColor(ofColor::white);
	ofFill();
	float lineWidth = ofMap(roadLineWidth, 0, 100, 0, roadTexWidth, true);
	float lineHeight = ofMap(roadLineHeight, 0, 100, 0, roadTexHeight, true);
	ofDrawRectangle((roadTexWidth - lineWidth) / 2.0, (roadTexHeight - lineHeight) / 2.0, 0
		, lineWidth, lineHeight);

	fboRoad.end();

	texRoad = fboRoad.getTexture();
}

//--------------------------------------------------------------
void ofxCity::setupRoad() {

	roads.clear();
	for (int i = 0; i < CITY_NUM_ROAD_PLANES; i++) {
		ofPlanePrimitive plane = ofPlanePrimitive(roadTexWidth, roadTexHeight, 2, 2);
		plane.resizeToTexture(texRoad);
		plane.rotate(90, 1, 0, 0);
		plane.setPosition(0, 0, -roadTexHeight * i);
		roads.push_back(plane);
	}
}

//--------------------------------------------------------------
void ofxCity::updateRoad(bool createNewRow) {

	
	// if plane is offsight
	// translate all planes along negative z axis
	if (createNewRow) {
		cout << "r" << endl;


		// translate all road planes
		for (auto & plane : roads) {

			ofVec3f pos = plane.getPosition();
			pos.z -= (curDistanceOffset);
			plane.setPosition(pos);
		}

		curDistanceOffset = 0;
	}
}

//--------------------------------------------------------------
void ofxCity::setupBlocks() {

	blocksL.resize(CITY_BLOCKS_ROWS * CITY_BLOCKS_COLS);
	blocksR.resize(CITY_BLOCKS_ROWS * CITY_BLOCKS_COLS);

	for (int row = 0; row < CITY_BLOCKS_ROWS; row++) {
		for (int col = 0; col < CITY_BLOCKS_COLS; col++) {

			blocksL[row * CITY_BLOCKS_COLS + col] = 0;
			blocksR[row * CITY_BLOCKS_COLS + col] = 0;

		}
	}
}

//--------------------------------------------------------------
void ofxCity::updateBlocks(int createRowsCount) {

	if (autoGenerateBuildings) {

		for (int i = 0; i < createRowsCount; i++) {

			translateBlocksHeights();

			if (i > 0) {
				// Translate all heights one row down
				translateBlocksHeights();
			}

			generateBlockSide(true, 1);
			generateBlockSide(false, 1);
		}
	}

	//if (createRowsCount > 1 /* forced */) {
	//	generateBlock_TheBigOne();
	//}
}

//--------------------------------------------------------------
void ofxCity::translateBlocksHeights() {

	// Translate all heights one row down
	for (int col = 0; col < CITY_BLOCKS_COLS; col++) {
		for (int row = 1; row < CITY_BLOCKS_ROWS; row++) {

			blocksL[(row - 1) * CITY_BLOCKS_COLS + col] = blocksL[row * CITY_BLOCKS_COLS + col];
			blocksR[(row - 1) * CITY_BLOCKS_COLS + col] = blocksR[row * CITY_BLOCKS_COLS + col];

			if (row == CITY_BLOCKS_ROWS - 1) {
				blocksL[row * CITY_BLOCKS_COLS + col] = 0;
				blocksR[row * CITY_BLOCKS_COLS + col] = 0;
			}

		}
	}

}

//--------------------------------------------------------------
void ofxCity::generateBlockSide(bool isLeftSide, int nowRowForced) {

	vector<int> &blocks = isLeftSide ? blocksL : blocksR;

	if (blockProbability > 0) {

		int numCols = 0;
		int lastReservedCol = -1;


		while (ofRandom(1) > (1 - blockProbability) && numCols++ < CITY_BLOCKS_COLS) {
			// DBG
			//while (true > (1 - blockProbability) && numCols++ < CITY_BLOCKS_COLS) {
			// create block

			// search next available place
			for (int col = lastReservedCol + 1; col < CITY_BLOCKS_COLS; col++) {
				if (blocks[col] > 0) {
					if (blockProbability < 1) {
						col += ofRandom(0, CITY_BLOCKS_COLS - col - 1);
					}
					lastReservedCol = col;
				}
				else
				{
					// found empty block


					int requestedCols = (int)ceilf(ofRandom(0, CITY_BLOCKS_ROWS));
					int requestedRows = (int)ceilf(ofRandom(0, CITY_BLOCKS_COLS));
					// DBG
					//int requestedCols = 1;//(int)ceilf(ofRandom(0,CITY_BLOCKS_ROWS));
					//int requestedRows = isLeftSide ? 1 : 3;// (int)ceilf(ofRandom(0,CITY_BLOCKS_ROWS));
					ofLogVerbose("ofxCity") << "building at col " << col << " cols=" << requestedCols << " rows=" << requestedRows;

					int minRow = 0, minCol = col, maxCol = 0, maxRow = 0;

					float height = ofRandom(100, ofMap(blockProbability, 0, 1, blockProbability / 10, blockProbability  *blockProbability) * CITY_BLOCK_MAXHEIGHT);
					for (int i = col; i < min(col + requestedCols, CITY_BLOCKS_COLS); i++) {

						if (true) {
							for (int j = 0; j < min(requestedRows + 1, CITY_BLOCKS_ROWS); j++) {

								maxRow = j;
								int index = j * CITY_BLOCKS_COLS + i;

								blocks[index] = height;

							}
						}

						lastReservedCol = max(lastReservedCol, i);
						maxCol = i;
					}

					// Create building
					float w = 0, h = 0, x = 0, z = 0;
					ofLogVerbose("ofxCity") << "got Col min/max= " << minCol << "/" << maxCol << " row min/max" << minRow << "/" << maxRow;
					float margin = CITY_BLOCK_SIZE * ofRandom(0, CITY_BLOCK_MARGIN_FACTOR);
					w = (maxCol - minCol + 1) * CITY_BLOCK_SIZE - 2 * margin;
					h = (maxRow - minRow + 1) * CITY_BLOCK_SIZE - 2 * margin;
					int roadPlaneIndex = 0; // CITY_NUM_ROAD_PLANES - 1
					if (isLeftSide) {
						x = -((w + roads[roadPlaneIndex].getWidth()) / 2.0 + minCol * CITY_BLOCK_SIZE + ofRandom(10, CITY_BLOCK_PAVEMENT_SIZE));
					}
					else {

						x = +((w + roads[roadPlaneIndex].getWidth()) / 2.0 + minCol * CITY_BLOCK_SIZE + ofRandom(10, CITY_BLOCK_PAVEMENT_SIZE));
					}
					z = roads[roadPlaneIndex].getPosition().z - h / 2. - CITY_BLOCK_SIZE * nowRowForced;
					ofBuilding building = ofBuilding(ofVec3f(x, -height / 2., z), w, height, h);
					buildings.push_back(building);
					ofLogVerbose("ofxCity") << "added building width=" << w << ", depth=" << h << ", height=" << height << " at (x,z) = " << x << ", " << z;
				}
				break; // for (int col = lastReservedCol
			}
		}
	}
}

//--------------------------------------------------------------
void ofxCity::update() {

	float now = ofGetElapsedTimef();

	float hash = roadLineWidth * 5.
		+ roadLineHeight * 7.
		+ roadTexWidth * 11.
		+ roadTexHeight * 13.;
	if (hash != roadParamsHash)
	{
		setupTextures();

		roadParamsHash = hash;
	}

	ofApp *app = (ofApp *)ofxGetAppPtr();
	if (bUpdateParamsFromCode) {
		app->transformer.cam.setFov(fov);
//        app->transformer.cam.setOrientation(camOrientation);
//        app->transformer.cam.setPosition(camPosition);

		bUpdateParamsFromCode = false;
	}

	if (bTweenSpeed) {
		curSpeed = ofxeasing::map(now, speedStartTime, speedEndTime, speedFrom, speedTo, &ofxeasing::sine::easeInOut);
	}

	if (mode != enCityExplosion) {

		if (curDistanceOffset <= -roadTexHeight) {
			updateRoad(true);
			updateBlocks(1);
		}


		curDistance -= curSpeed;
		curDistanceOffset -= curSpeed;

		directionalLight.setDiffuseColor(diffuseColor);
		directionalLight.setSpecularColor(specularColor);
		directionalLight.setAmbientColor(ambientColor);
		directionalLight.setDirectional();
		directionalLight.setOrientation((ofVec3f)dirLightOrientation);

		int i = 0;
		float roadNearZ = roads[0].getPosition().z;
		float roadFarZ = roads[CITY_NUM_ROAD_PLANES - 1].getPosition().z;
		while (i < buildings.size()) {
			if (buildings[i].position.z < roadFarZ + 650) {
				buildings[i] = buildings.back(); buildings.pop_back(); i--;
			}
			else {
				i++;
			}
		}

	}

	switch (mode) {
	case enCityCollapsing:
		if (now > transEndTime)
		{
			cout << "COLLAPSED" << endl;
			mode = enCityCollapsed;
		}
		break;
	case enCityExplosion:
		updateExplosion();
		break;
	}



}

void ofxCity::captureCam() {
	ofApp *app = (ofApp *)ofxGetAppPtr();
	camOrientation = app->transformer.cam.getOrientationEuler();
	camPosition = app->transformer.cam.getPosition();
	//gui.saveToFile(Globals::hostName + CITY_SETTINGS_FILE);
}


//--------------------------------------------------------------
int roadDivision = 4;
int numBuildingsExMin = 4;
int numBuildingsExMax = 1000;
vector<ofVec3f> roadPosEx;
vector<ofVec3f> buildingsPosEx;
vector<ofVec2f> buildingsSizesEx;
float BUILDING_MAX_SIZE_EX = 150;
void ofxCity::setupExplosion() {

	float now = ofGetElapsedTimef();
	
	ofApp *app = (ofApp *)ofxGetAppPtr();
	app->transformer.cam.setPosition(app->transformer.cam.getPosition() + ofVec3f(0, 800, 0));
	decelerate(2000, true);
	roadPosEx.clear();
	buildingsPosEx.clear();
	buildingsSizesEx.clear();
	
	explosionFrom = 0;
	explosionTo = 1;
	explosionInitTime = now;
	explosionEndTime = now + 10000;
	//explosionTween.setParameters(easingexpo, ofxTween::easeInOut, 0, 1, 10000,0);

	float roadSize = roadTexWidth / roadDivision;

	for (int i = 0; i < roads.size(); i++) {
		ofVec3f pos = roads[i].getPosition();

		// road squares
 		for (int x = 0; x < roadDivision; x++) {
			for (int y = 0; y < roadDivision; y++) {
				roadPosEx.push_back(pos + ofVec3f(x * (roadTexWidth / roadDivision), 0, y * (roadTexWidth / roadDivision)));
				//roadPosEx.push_back(pos + ofVec3f(x * (roadTexWidth / roadDivision), 0, y * (roadTexWidth / roadDivision)));
			}
		}

		// random buildings
		for (int j = 0; j < (int)ofMap(ofRandom(1), 0, 1, numBuildingsExMin, numBuildingsExMax); j++) {
			if (ofRandomf() > 0.5) {
				buildingsPosEx.push_back(ofVec3f(pos.x  - ofRandom(10, 100) * j - roadSize,
					0,
					pos.z - ofRandom(-100, 100)));
				buildingsSizesEx.push_back(ofVec2f(ofRandom(10, BUILDING_MAX_SIZE_EX), ofRandom(10, roadSize * 2)));
			}
		}
		for (int j = 0; j < (int)ofMap(ofRandom(1), 0, 1, numBuildingsExMin, numBuildingsExMax); j++) {
			if (ofRandomf() > 0.5) {
				buildingsPosEx.push_back(ofVec3f(pos.x + ofRandom(10, 100) * j + roadSize,
					0,
					pos.z - ofRandom(-100, 100)));
				buildingsSizesEx.push_back(ofVec2f(ofRandom(10, BUILDING_MAX_SIZE_EX), ofRandom(10, BUILDING_MAX_SIZE_EX)));
			}
		}
	}
	//roads.clear();
}
//--------------------------------------------------------------
float amount;
void ofxCity::updateExplosion() {
	
	float now = ofGetElapsedTimef();
	
	prevCamTranslate = curCamTranslate;
	prevCamRot = curCamRot;
	
	// generate a noisy 3d position over time
	float t = (2 + ofGetElapsedTimef()) * .01;
	curCamTranslate.x = ofSignedNoise(t-6.12, 0, 0);
	curCamTranslate.y = ofSignedNoise(0, t+0.78, 0);
	curCamTranslate.z = ofSignedNoise(0, 0, t-9.76);
	curCamTranslate *= 100 ; // scale from -1,+1 range to -400,+400
	
	// generate a noisy 3d rotation over time
	t = (5.6 + ofGetElapsedTimef()) * .01;
	curCamRot.x = ofSignedNoise(t, 0, 0);
	curCamRot.y = ofSignedNoise(0, t+1.23, 0);
	curCamRot.z = ofSignedNoise(0, 0, t-4.5);
	curCamRot *= 15 ; // scale from -1,+1 range to -400,+400

	
	amount = ofxeasing::map(now, explosionInitTime, explosionEndTime, explosionFrom, explosionTo, &ofxeasing::exp::easeInOut);	//amount = 0;

	for (int i = 0; i < roadPosEx.size(); i++) {
		roadPosEx[i].y = -(ofNoise(ofGetElapsedTimef() * 0.02 + 1.23 + i) * CITY_BLOCK_MAXHEIGHT * amount );
		roadPosEx[i].x += ofSignedNoise(ofGetElapsedTimef() * 0.013 - 1.23 - i)* amount;
		roadPosEx[i].z += ofSignedNoise(ofGetElapsedTimef() * 0.0024 + 0.0893 + i)* amount;
							//+ amount * CITY_BLOCK_MAXHEIGHT * ofSignedNoise(ofGetElapsedTimef() * 0.12 + 4.7+i));
	}

	ofSetColor(ofColor::red);
	for (int i = 0; i < buildingsPosEx.size(); i++) {

		buildingsPosEx[i].y = -(ofNoise(ofGetElapsedTimef() * 0.012 - 0.23 + i) * CITY_BLOCK_MAXHEIGHT  *amount);

		buildingsPosEx[i].x += ofSignedNoise(ofGetElapsedTimef() * 0.098 - 0.883 - 5.3*i)* amount;
		buildingsPosEx[i].z += ofSignedNoise(ofGetElapsedTimef() * 0.0036 + 126.5* i)* amount;
			//+ amount*  CITY_BLOCK_MAXHEIGHT * ofSignedNoise(ofGetElapsedTimef() * 0.17  + 7.48-i));
	}
}
//--------------------------------------------------------------
void ofxCity::drawExplosion() {

	ofFill();	

	//ofDrawAxis(500);

	ofPushMatrix();
	ofRotateDeg(0, 0, 0, 0);
	ofTranslate(curCamTranslate.x, curCamTranslate.y, curCamTranslate.z);
	ofRotateXDeg(curCamRot.x);
	ofRotateYDeg(curCamRot.y);
	ofRotateZDeg(curCamRot.z);

	ofSetColor(ofColor::gray);
	float roadSize = roadTexWidth / roadDivision;
	
	for (int i = 0; i < roadPosEx.size(); i++) {
		
		float noiseAngle = 45 * ofSignedNoise(ofGetElapsedTimef() * 0.3 -4.2*i)* amount;
		float noisex = ofSignedNoise(ofGetElapsedTimef() * 0.047 +6.3*i)* amount;
		float noisey = ofSignedNoise(ofGetElapsedTimef() * 0.033 - i*4+123.1)* amount;
		float noisez = ofSignedNoise(ofGetElapsedTimef() * 0.093 + 63.3 + i *12.3)* amount;
		
		ofPushMatrix();
			ofTranslate(roadPosEx[i]);
			ofRotateDeg(noiseAngle, noisex, noisey, noisez);
			ofPushMatrix();
			ofRotateDeg(90, 1, 0, 0);
				ofDrawRectangle(0, 0, roadSize, roadSize);
			ofPopMatrix();
		ofPopMatrix();

	}
	

	ofSetColor(ofColor::red);
	for (int i = 0; i < buildingsPosEx.size(); i++) {

		float noiseAngle = 45 * ofSignedNoise(ofGetElapsedTimef() * 0.048 + 12 + 4.2*i)* amount;

		float noisex = ofSignedNoise(ofGetElapsedTimef() * 0.048 + 0.69*i)* amount;
		float noisey = ofSignedNoise(ofGetElapsedTimef() * 0.097 - i * 1.2 - 13.1)* amount;
		float noisez = ofSignedNoise(ofGetElapsedTimef() * 0.077 - i * 4 + 123.1)* amount;

		ofPushMatrix();
			ofTranslate(buildingsPosEx[i]);
			ofRotateDeg(noiseAngle, noisex, noisey, noisez);
			ofPushMatrix();
			ofRotateDeg(90, 1, 0, 0);
				ofDrawRectangle(0, 0, buildingsSizesEx[i].x, buildingsSizesEx[i].y);
			ofPopMatrix();
		ofPopMatrix();
	}
	ofPopMatrix();

}

void ofxCity::accelerate(int duration) {
	desiredSpeed += CITY_SPEED_INCR;
	speedStartTime = ofGetElapsedTimef();
	speedEndTime = speedEndTime + duration;
	speedFrom = curSpeed;
	speedTo = desiredSpeed;
}
void ofxCity::decelerate(int duration, bool stop) {
	if (stop) {
		desiredSpeed -= curSpeed;
	}
	else {
		desiredSpeed -= CITY_SPEED_INCR;
	}
	speedStartTime = ofGetElapsedTimef();
	speedEndTime = speedEndTime + duration;
	speedFrom = curSpeed;
	speedTo = desiredSpeed;
}

//--------------------------------------------------------------
void ofxCity::draw() {

	ofSetSmoothLighting(false);
	ofEnableAlphaBlending();
	ofEnableDepthTest();

	float now = ofGetElapsedTimef();

	switch (mode)
	{
	case enCityCollapsing:
		{
			float col = ofxeasing::map(now, collapseInitTime, collapseEndTime, collapseFrom, collapseTo, ofxeasing::linear::easeIn);
		ofBackground(col);
		}
			break;
	default:
		//ofBackground(255, 255, 255, 255);
		ofBackground(255);
		break;
	}

	
	if (mode == enCityLine) {

		ofSetColor(0);
		ofFill();
		ofPushMatrix();

		int rW = 900;
		int rH = 75;
		ofTranslate(ofGetWidth() / 2 - rW /2, ofGetHeight() / 2 - rH / 2);

		ofDrawRectangle(0, 0, rW, rH);
		ofPopMatrix();
    } else if (mode == enCityBlank) {
        // nothing
    }
	else
	{
		ofPushMatrix();

		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		ofTranslate(0, 20);
		ofTranslate(0, 0, curDistance + 650);

		switch (mode)
		{
		case enCityIdle:
			break;
		case enCityCollapsed:
			ofClear(0);
			break;
		case enCityExplosion:
			drawExplosion();
			break;
		default:
		{
			texRoad.bind();
			for (int i = 0; i < roads.size(); i++) {

				float alpha = ofMap(i, CITY_NUM_ROAD_PLANES - CITY_NUM_ROAD_PLANES_FADEIN, CITY_NUM_ROAD_PLANES - 1, 255, 0, true);
				ofSetColor(ofColor::white, alpha);
				roads[i].draw();
			}
			texRoad.unbind();





			//------------------------
			// Terrain draw
			//
			/*ofPushMatrix();
			ofRotateDeg(90, 1, 0, 0);
			ofTranslate(0, -10000, 0);
			terrain.draw();
			ofPopMatrix();*/
			//
			//

			ofEnableLighting();
			directionalLight.enable();
			material.begin();

			ofFill();

			int maxDepth = CITY_BLOCKS_ROWS * CITY_BLOCK_SIZE + 100;
			int minDepth = maxDepth + 500;//+2000;
			float roadLength = CITY_NUM_ROAD_PLANES * texRoad.getHeight();

			float road0z = roads[0].getPosition().z + 650;
			float roadnz = roads[CITY_NUM_ROAD_PLANES - 1].getPosition().z;

			if (mode == enCityCollapsing) {
				//rotationAngle = tweenRotate.update();
				translationCollapse = ofxeasing::map(now, transInitTime, transEndTime, transFrom, transTo, ofxeasing::exp::easeIn);


				float boxW = ofxeasing::map(now, boxWInitTime, boxWEndTime, boxWFrom, boxWTo, ofxeasing::exp::easeIn);
				float boxH = ofxeasing::map(now, boxHInitTime, boxHEndTime, boxHFrom, boxHTo, ofxeasing::exp::easeIn);
				ofPushMatrix();
				ofTranslate(-boxW / 2, -boxH / 2, -1000);

				ofSetColor(0);
				ofDrawRectangle(0, 0, boxW, boxH);

				ofPopMatrix();

			}

			/*ofPushMatrix();
			ofTranslate(0, 0, translationCollapse);*/
			if (mode != enCityCollapsed) {

				for (std::vector<ofBuilding>::iterator buildingIt = buildings.begin(); buildingIt != buildings.end(); ++buildingIt) {
					ofBuilding building = *buildingIt;
					if (building.position.z < road0z) {
						int curDepth = building.position.z + roadLength + curDistance - CITY_BLOCK_SIZE;
						float alpha = ofMap(curDepth, minDepth, maxDepth, 255, 0, true);
						ofSetColor(ofColor::white, alpha);
						building.draw(rotationAngle, translationCollapse, alpha, bWireframe);
					}

				}
			}

			//ofPopMatrix();

			ofFill();
			ofSetColor(255);
			ofSetLineWidth(1);

			material.end();
			directionalLight.disable();
			ofDisableLighting();


			/* Test box
			ofSetColor(ofColor::red);
			ofBoxPrimitive box = ofBoxPrimitive(100,500,100);
			box.setPosition(0,0,0);
			box.drawWireframe();*/
			ofDisableDepthTest();
			ofDisableAlphaBlending();

		}
		break;
		}

		ofPopMatrix();
	}


	ofApp *app = (ofApp *)ofxGetAppPtr();
	app->transformer.cam.end();
	if (bShowGui) {
		ofDisableDepthTest();

		gui.draw();
	}

	

	if (debugFbo) {

		stringstream ss;
		ss << "FPS : " + ofToString(ofGetFrameRate());
		ofDrawBitmapStringHighlight(ss.str(), 10, 10);

		for (int x = 0; x < CITY_BLOCKS_COLS; x++) {
			for (int y = 0; y < CITY_BLOCKS_ROWS; y++) {
				img.setColor(CITY_BLOCKS_COLS + 1, y, ofColor::red);
				img.setColor(CITY_BLOCKS_COLS + 2, y, ofColor::red);
				img.setColor(CITY_BLOCKS_COLS + 3, y, ofColor::red);
				img.setColor(CITY_BLOCKS_COLS + 4, y, ofColor::red);
				img.setColor(CITY_BLOCKS_COLS + 5, y, ofColor::red);

				ofColor color = ofColor::black;
				if (blocksL[y * CITY_BLOCKS_COLS + x] > 0) {
					color = ofColor::white;
				}
				/*img.setColor((CITY_BLOCKS_COLS - x),(CITY_BLOCKS_ROWS- y),color);*/
				img.setColor(CITY_BLOCKS_COLS - x, y, color);

				color = ofColor::black;
				if (blocksR[y * CITY_BLOCKS_COLS + x] > 0) {
					color = ofColor::white;
				}
				/*img.setColor((CITY_BLOCKS_COLS - x) + CITY_BLOCKS_COLS + 5,(CITY_BLOCKS_ROWS- y),color);*/
				img.setColor(x + CITY_BLOCKS_COLS + 5, y, color);

			}
		}
		img.update();
		img.draw(100, 200, CITY_BLOCKS_COLS * 10, -CITY_BLOCKS_ROWS * 10);
	}

	app->transformer.cam.begin();


}



//--------------------------------------------------------------
void ofxCity::keyPressed(int key) {
	ofApp *app = (ofApp *)ofxGetAppPtr();

	switch (key)
	{
	case ' ':
		mode = (mode + 1) % 9;
		setMode(mode);
		break;
	case 'p': bUpdateParamsFromCode = !bUpdateParamsFromCode; break;
	case 'P': captureCam(); break;
	case 'r': setup(); break;
        case 'l': {
            gui.loadFromFile(Globals::hostName + CITY_SETTINGS_FILE);
            ofSeedRandom(123456);
            setupTextures();
        } break;
	case 's': gui.saveToFile(Globals::hostName + CITY_SETTINGS_FILE); break;
	case 'h':
		bShowGui = !bShowGui;
		if (bShowGui) {
			app->transformer.cam.disableMouseInput();
		}
		else {
			app->transformer.cam.enableMouseInput();
		}
		break;
	case 'z':
		accelerate();
		break;
	case 'Z':
		decelerate();
		break;
	case 'b':
		updateBlocks(1);
		break;
	case '<':
		blockProbability = ofClamp(blockProbability - 0.1, 0, 1);
		break;
	case '>':
		blockProbability = ofClamp(blockProbability + 0.1, 0, 1);
		break;
	case 'w':
		autoGenerateBuildings = !autoGenerateBuildings;
		break;
	case 'e':
		//forceShader = !forceShader;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofxCity::setMode(int mode) {

	float now = ofGetElapsedTimef();
	
	switch (mode) {
	case enCityStart:
		accelerate(0); // start with velocity
		break;
	case enCityBuildings:
		autoGenerateBuildings = true;
		break;
	case enCityCollapsing:
	{
		float lowestZ = 100000;
		float highestZ = -10000;
		for (std::vector<ofBuilding>::iterator buildingIt = buildings.begin(); buildingIt != buildings.end(); ++buildingIt) {
			ofBuilding building = *buildingIt;
			lowestZ = min(lowestZ, building.position.z);
			highestZ = max(highestZ, building.position.z);
		}
		float diffZ = highestZ - lowestZ;

		int collapseTime = 15;
		//tweenRotate.setParameters(easingexpo, ofxTween::easeIn, 0, 0, collapseTimeMs, 0);

		float roadNz = roads[CITY_NUM_ROAD_PLANES - 1].getPosition().z;
		float road0z = roads[0].getPosition().z;

		float initialZ = lowestZ - 650;
		float zOffsetAtEnd = ofGetFrameRate() * (curDistanceOffset)* collapseTime;
		transInitTime = now;
		transEndTime = now + collapseTime;
		transFrom = 0;
		transTo =-zOffsetAtEnd + lowestZ - roadNz;
		
		collapseFrom = 255;
		collapseTo = 0;
		collapseInitTime = now + 3;
		collapseEndTime = collapseInitTime + collapseTime / 5.;
		
		boxWInitTime = now;
		boxWEndTime = now + collapseTime / 20.;
		boxWFrom = 0;
		boxWTo = CITY_COLLAPSE_BOX_WIDTH;
		boxHInitTime = now;
		boxHEndTime = collapseTime;
		boxHFrom = 0;
		boxHTo = CITY_COLLAPSE_BOX_HEIGHT;
	}
	break;
        case enCityAgain: {
            gui.loadFromFile(Globals::hostName + CITY_SETTINGS_FILE);
            setupTextures();
            bUpdateParamsFromCode = true;
            
        } break;
	case enCityExplosion: {
		setupExplosion();
	}
						  break;
        case enCityLine: {
            ofApp *app = (ofApp *)ofxGetAppPtr();
            app->transformer.cam.reset();
        } break;
	}
	cout << ofToString(mode) << endl;
}
