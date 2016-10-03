#include "ofxCity.h"
#include "ofApp.h"

//--------------------------------------------------------------
void ofxCity::setup(){

	ofSeedRandom(123456);	
	bUpdateParamsFromCode = true;

	// shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess( 120 );
	material.setSpecularColor(ofColor(255, 255, 255, 255));

	img.allocate(CITY_BLOCKS_COLS * 2 + 5, CITY_BLOCKS_ROWS, ofImageType::OF_IMAGE_COLOR);

	ofApp *app = (ofApp *)ofxGetAppPtr();
	app->cam.reset();

	app->cam.setFarClip(100000);

	mode = enCityIdle;
	curSpeed = 0;
	desiredSpeed = 0;
	curDistanceOffset = 0;
	curDistance = 0;
	rotationAngle = 0;
	buildings.clear();
	tween.setParameters(easinglinear, ofxTween::easeInOut
		, curSpeed
		, desiredSpeed
		, 0,0);

	// GUI
	roadParamsHash = 0;
	if (!bGuiLoaded) {
		gui.setup("cityPanel",CITY_SETTINGS_FILE); // most of the time you don't need a name but don't forget to call setup
		gui.add(bWireframe.set("Wireframe", false));
		gui.add(bTweenSpeed.set("Tween speed", true));
		gui.add(fov.set("FOV", 60,0,360));
		gui.add(debugFbo.set("Debug FBO", false));
		gui.add(camOrientation.set("camOrientation",app->cam.getOrientationEuler(), ofVec3f(-180,-180,-180), ofVec3f(180,180,180)));
		gui.add(camPosition.set("camPosition",app->cam.getPosition(), ofVec3f(-180,-180,-180), ofVec3f(180,180,180)));
		roadParams.setName("Road");

		roadParams.add(rotationAngle.set("rotationAngle", 0 ,-180 , 180));
		roadParams.add(roadOpacity.set("Road opacity", 0 ,0 , 255));
		roadParams.add(roadTexWidth.set("Road tex width", 100,10,1000));
		roadParams.add(roadTexHeight.set("Road tex height", 100,10,1000));		
		roadParams.add(roadLineWidth.set("Road line width", 1.5,1,100));
		roadParams.add(roadLineHeight.set("Road line height", 50,1,100));
		roadParams.add(curSpeed.set("Velocity", 0, 0, 150));
		gui.add(roadParams);

		buildingParams.setName("Buildings");
		buildingParams.add(blockProbability.set("Proba",1,0,1));
		buildingParams.add(autoGenerateBuildings.set("Auto generated", false));
		gui.add(buildingParams);

		lightParams.setName("Lights");
		lightParams.add(dirLightOrientation.set("Orientation (x,z,y angles)", ofVec3f(180, -42.2449, 117.551), ofVec3f(-180), ofVec3f(180)));
		lightParams.add(diffuseColor.set("diffuseColor", ofFloatColor(1, 0, 0, 1),ofFloatColor(0,0,0,0),ofColor::white));
		lightParams.add(specularColor.set("specularColor", ofFloatColor(1, 0, 0, 1),ofFloatColor(0,0,0,0),ofColor::white));
		lightParams.add(ambientColor.set("ambientColor", ofFloatColor(0), ofFloatColor(0,0,0,0),ofColor::white));
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
void ofxCity::setupTextures(){

	fboRoad.allocate(roadTexWidth,roadTexHeight, GL_RGB32F_ARB);
	fboRoad.begin();

	ofBackground(ofColor::darkSlateGray);
	ofSetColor(ofColor::white);
	ofFill();
	float lineWidth = ofMap(roadLineWidth, 0, 100, 0, roadTexWidth, true);
	float lineHeight = ofMap(roadLineHeight, 0, 100, 0, roadTexHeight, true);
	ofRect((roadTexWidth - lineWidth) / 2.0, (roadTexHeight - lineHeight) / 2.0, 0
		, lineWidth, lineHeight);

	fboRoad.end();

	texRoad = fboRoad.getTextureReference();
}

//--------------------------------------------------------------
void ofxCity::setupRoad(){

	roads.clear();
	for (int i=0; i < CITY_NUM_ROAD_PLANES; i++) {
		ofPlanePrimitive plane = ofPlanePrimitive(roadTexWidth, roadTexHeight, 2, 2);
		plane.resizeToTexture( texRoad );
		plane.rotate(90, 1, 0, 0);
		plane.setPosition(0,0,-roadTexHeight * i);
		roads.push_back(plane);
	}


}

//--------------------------------------------------------------
void ofxCity::updateRoad(bool createNewRow) {

	// if plane is offsight
	// translate all planes along negative z axis
	if (createNewRow) {
		cout << ".";


		// translate all road planes
		for(auto & plane: roads) {

			ofVec3f pos = plane.getPosition();
			pos.z -= curDistanceOffset-1;
			plane.setPosition(pos);
		}

		curDistanceOffset = 0;
	}


}

//--------------------------------------------------------------
void ofxCity::setupBlocks(){

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

	if (createRowsCount > 1 /* forced */ || autoGenerateBuildings) {

		for (int numIterations = 0; numIterations < createRowsCount; numIterations++) {

			translateBlocksHeights(); 

			if (numIterations > 0) {
				// Translate all heights one row down
				translateBlocksHeights();
				translateBlocksHeights();
				translateBlocksHeights();
			}

			generateBlockSide(true, numIterations);
			generateBlockSide(false, numIterations);
		}
	}

	if (createRowsCount > 1 /* forced */) {
		generateBlock_TheBigOne();
	}
}

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
			for (int col = lastReservedCol+1; col < CITY_BLOCKS_COLS; col++) {
				if (blocks[col] > 0) {
					lastReservedCol = col;
				}
				else
				{
					// found empty block


					int requestedCols = (int)ceilf(ofRandom(0,CITY_BLOCKS_ROWS));
					int requestedRows = (int)ceilf(ofRandom(0,CITY_BLOCKS_COLS));
					// DBG
					//int requestedCols = 1;//(int)ceilf(ofRandom(0,CITY_BLOCKS_ROWS));
					//int requestedRows = isLeftSide ? 1 : 3;// (int)ceilf(ofRandom(0,CITY_BLOCKS_ROWS));
					ofLogVerbose("ofxCity")  << "building at col " << col << " cols=" << requestedCols << " rows=" << requestedRows;

					int minRow = 0, minCol = col, maxCol = 0, maxRow = 0;

					float height = ofRandom(100, blockProbability * blockProbability * CITY_BLOCK_MAXHEIGHT);
					for (int i = col; i < min(col + requestedCols, CITY_BLOCKS_COLS); i++) {

						if (true) {
							for (int j = 0; j < min(requestedRows+1, CITY_BLOCKS_ROWS); j++) {

								maxRow = j;
								int index = j * CITY_BLOCKS_COLS + i;

								blocks[index] = height;

							}
						}

						lastReservedCol = max(lastReservedCol,i);
						maxCol = i;						
					}

					// Create building
					float w = 0, h = 0, x = 0, z = 0;
					ofLogVerbose("ofxCity")  << "got Col min/max= " << minCol << "/" << maxCol << " row min/max" << minRow << "/" << maxRow;
					float margin = CITY_BLOCK_SIZE * ofRandom(0,CITY_BLOCK_MARGIN_FACTOR);
					w = (maxCol - minCol + 1) * CITY_BLOCK_SIZE - 2 * margin;
					h = (maxRow - minRow + 1) * CITY_BLOCK_SIZE - 2 * margin;
					if (isLeftSide) {
						x = - ((w + roads[CITY_NUM_ROAD_PLANES-1].getWidth()) / 2.0 + minCol * CITY_BLOCK_SIZE + ofRandom(10,CITY_BLOCK_PAVEMENT_SIZE));
					} else {

						x = + ((w + roads[CITY_NUM_ROAD_PLANES-1].getWidth()) / 2.0 + minCol * CITY_BLOCK_SIZE + ofRandom(10,CITY_BLOCK_PAVEMENT_SIZE));
					}
					z = roads[CITY_NUM_ROAD_PLANES-1].getPosition().z - h /2. - CITY_BLOCK_SIZE * nowRowForced;
					ofBuilding building = ofBuilding(ofVec3f(x, -height / 2., z), w, height, h);
					buildings.push_back(building);
					ofLogVerbose("ofxCity")  << "added building width=" << w << ", depth=" << h << ", height=" << height << " at (x,z) = " << x << ", " << z;
				}
				break; // for (int col = lastReservedCol
			}
		}
	}
}

//--------------------------------------------------------------
void ofxCity::generateBlock_TheBigOne() {

	float size = 5000;

	float lowestZ = 0;
	for(std::vector<ofBuilding>::iterator buildingIt = buildings.begin(); buildingIt != buildings.end(); ++buildingIt) {
		ofBuilding building = *buildingIt;
		lowestZ = min(lowestZ,building.position.z);
	}
	
	ofBuilding building = ofBuilding(ofVec3f(0,-CITY_BLOCK_MAXHEIGHT / 2,lowestZ-1000),size*2, CITY_BLOCK_MAXHEIGHT,  size/4);
			buildings.push_back(building);

}

//--------------------------------------------------------------
void ofxCity::update(){
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
		app->cam.setFov(fov);
		app->cam.setOrientation(camOrientation);
		app->cam.setPosition(camPosition);
	}

	if (curDistanceOffset >= roadTexHeight) {
		updateRoad(true);
		updateBlocks(false);
	}

	if (bTweenSpeed && bUpdateParamsFromCode) {
		curSpeed = tween.update();
	}
	curDistance += curSpeed;
	curDistanceOffset += curSpeed;

	directionalLight.setDiffuseColor(diffuseColor);
	directionalLight.setSpecularColor(specularColor);
	directionalLight.setAmbientColor(ambientColor);
	directionalLight.setDirectional();
	directionalLight.setOrientation((ofVec3f)dirLightOrientation);

	int i = 0;
	float road0z = roads[0].getPosition().z;
	while (i < buildings.size()) {
		if (buildings[i].position.z >= road0z + 650) {
			buildings[i] = buildings.back(); buildings.pop_back(); i--;
		} else {
			i++;
		}
	}
}

//--------------------------------------------------------------
void ofxCity::draw(){


	ofSetSmoothLighting(true);
	ofEnableAlphaBlending();
	ofEnableDepthTest();

	ofBackground(255,255,255,255);

	ofPushMatrix();
	texRoad.bind();

	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	ofTranslate(0, 20);
	if (tweenRotate.isCompleted() && mode == enCityCollapsing){
		float boomAmp = 30;
		ofTranslate(ofRandomf()*boomAmp,0,ofRandomf()*boomAmp);
	}

	ofTranslate(0, 0, curDistance + 650);

	if (mode != enCityIdle) {

		for (int i = 0; i < roads.size(); i++) {

			float alpha = ofMap(i,CITY_NUM_ROAD_PLANES-CITY_NUM_ROAD_PLANES_FADEIN,CITY_NUM_ROAD_PLANES-1,255,0,true);
			if (mode == enCityStart) {
				ofSetColor(ofColor::white, tweenRoadOpactity.update() * ofMap(alpha, 0,255,0,1));
			}
			else
			{
				ofSetColor(ofColor::white, alpha);
			}
			roads[i].draw();
		}

		texRoad.unbind();

		ofEnableLighting();
		directionalLight.enable();
		material.begin();

		ofFill();
		ofSetColor(255);

		int maxDepth = CITY_BLOCKS_ROWS * CITY_BLOCK_SIZE+100;
		int minDepth = maxDepth+500;//+2000;
		float roadLength =  CITY_NUM_ROAD_PLANES * texRoad.getHeight();

		float road0z = roads[0].getPosition().z + 650; 
		float roadnz = roads[CITY_NUM_ROAD_PLANES-1].getPosition().z;

		if (mode == enCityCollapsing) {
			rotationAngle = tweenRotate.update();
		}

		for(std::vector<ofBuilding>::iterator buildingIt = buildings.begin(); buildingIt != buildings.end(); ++buildingIt) {
			ofBuilding building = *buildingIt;
			if (building.position.z < road0z) {
				int curDepth = building.position.z + roadLength + curDistance - CITY_BLOCK_SIZE;
				float alpha = ofMap(curDepth,minDepth,maxDepth,255,0,true);
				ofSetColor(ofColor::white, alpha);
				building.draw(rotationAngle, alpha, bWireframe);
			}

		}


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


		ofPopMatrix();

	}

	ofApp *app = (ofApp *)ofxGetAppPtr();
	app->cam.end();
	if (bShowGui) {
		ofDisableDepthTest();
		gui.draw();
	}

	stringstream ss;
	ss << "FPS : " + ofToString(ofGetFrameRate());
	ofDrawBitmapStringHighlight(ss.str(), 10, 10);

	if (debugFbo) {

		for (int x = 0; x < CITY_BLOCKS_COLS; x++) {
			for (int y = 0; y < CITY_BLOCKS_ROWS; y++) {
				img.setColor(CITY_BLOCKS_COLS+1,y,ofColor::red);
				img.setColor(CITY_BLOCKS_COLS+2,y,ofColor::red);
				img.setColor(CITY_BLOCKS_COLS+3,y,ofColor::red);
				img.setColor(CITY_BLOCKS_COLS+4,y,ofColor::red);
				img.setColor(CITY_BLOCKS_COLS+5,y,ofColor::red);

				ofColor color = ofColor::black;
				if (blocksL[y * CITY_BLOCKS_COLS + x] > 0) {
					color = ofColor::white;
				}				
				/*img.setColor((CITY_BLOCKS_COLS - x),(CITY_BLOCKS_ROWS- y),color);*/
				img.setColor(CITY_BLOCKS_COLS - x,y,color);

				color = ofColor::black;
				if (blocksR[y * CITY_BLOCKS_COLS + x] > 0) {
					color = ofColor::white;
				}
				/*img.setColor((CITY_BLOCKS_COLS - x) + CITY_BLOCKS_COLS + 5,(CITY_BLOCKS_ROWS- y),color);*/
				img.setColor(x + CITY_BLOCKS_COLS + 5,y,color);

			}
		}
		img.update();
		img.draw(100,200,CITY_BLOCKS_COLS*10,-CITY_BLOCKS_ROWS*10);
	}


	app->cam.begin();

}

//--------------------------------------------------------------
void ofxCity::keyPressed(int key){
	ofApp *app = (ofApp *)ofxGetAppPtr();

	switch (key)
	{
	case ' ':
		mode = (mode + 1) % 4;
		switch(mode) {
		case enCityStart:
			tweenRoadOpactity.setParameters(easinglinear, ofxTween::easeInOut
				, 0
				, 255
				, 10000,0);
			break;
		case enCityBuildings:
			updateBlocks(3);
			break;
		case enCityCollapsing:
			tweenRotate.setParameters(easingexpo, ofxTween::easeIn
				, 0
				, 90
				, 7500,0);
			break;
		}
		cout << ofToString(mode) << endl;

		break;
	case 'p' : bUpdateParamsFromCode = !bUpdateParamsFromCode; break;
	case 'r': setup(); break;
	case 'l' : gui.loadFromFile(CITY_SETTINGS_FILE); break;
	case 's' : gui.saveToFile(CITY_SETTINGS_FILE); break;
	case 'h': 
		bShowGui = !bShowGui;
		if (bShowGui){
			app->cam.disableMouseInput();
		}
		else {
			app->cam.enableMouseInput();
		}			  
		break;
	case 'z': desiredSpeed += CITY_SPEED_INCR;
		tween.setParameters(easingsine, ofxTween::easeInOut
			, curSpeed
			, desiredSpeed
			, 2000,0);
		break;
	case 'b': 
		updateBlocks(true); 
		break;

	default:
		break;
	}
}