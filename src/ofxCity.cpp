#include "ofxCity.h"
#include "ofApp.h"

//--------------------------------------------------------------
void ofxCity::setup(){

	ofApp *app = (ofApp *)ofxGetAppPtr();
	app->cam.reset();
	curSpeed = 0;
	desiredSpeed = 0;
	curDistanceOffset = 0;
	curDistance = 0;
	tween.setParameters(easingsine, ofxTween::easeInOut
		, curSpeed
		, desiredSpeed
		, 0,0);

	// GUI
	roadParamsHash = 0;
	if (!bGuiLoaded) {
		gui.setup("cityPanel","city_settings.xml"); // most of the time you don't need a name but don't forget to call setup
		gui.add(bWireframe.set("Wireframe", false));
		roadParams.setName("Road params");
		roadParams.add(roadTexWidth.set("Road tex width", 100,10,1000));
		roadParams.add(roadTexHeight.set("Road tex height", 100,10,1000));		
		roadParams.add(roadLineWidth.set("Road line width %", 1.5,1,100));
		roadParams.add(roadLineHeight.set("Road line height %", 50,1,100));	
		roadParams.add(roadWidth.set("Road width", 100,10,1000));
		roadParams.add(roadHeight.set("Road height", 100,10,1000));

		bGuiLoaded = true;

		gui.add(roadParams);
	}
	bShowGui = false;

	setupTextures();

	setupRoad();
	setupBlocks();

}

//--------------------------------------------------------------
void ofxCity::setupTextures(){

	fboRoad.allocate(roadTexWidth,roadTexHeight);
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
void ofxCity::updateRoad(){

	// if plane is offsight
	// translate all planes along negative z axis
	if (curDistanceOffset >= roadTexHeight) {
		cout << "here" <<endl;

		// translate all road planes
		for(auto & plane: roads) {
			ofVec3f pos = plane.getPosition();
			pos.z -= curDistanceOffset;
			plane.setPosition(pos);
		}

		// generate new blocks on incoming line
		generateBlocks();

		curDistanceOffset = 0;
	}


}



//--------------------------------------------------------------
void ofxCity::setupBlocks(){

	blockProbability = 0;

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
void ofxCity::updateBlocks(){
	// translate all planes along negative z axis

	for (int col = 0; col < CITY_BLOCKS_COLS; col++) {
		for (int row = 0; row < CITY_BLOCKS_ROWS; row++) {

			blocksL[row * CITY_BLOCKS_COLS + col] = 0;
			blocksR[row * CITY_BLOCKS_COLS + col] = 0;

		}
	}
}

//--------------------------------------------------------------
void ofxCity::generateBlocks() {

	if (blockProbability > 0) {

		float rnd = ofRandom(1);

		if (rnd > 1 - blockProbability) {
			// create block

			// search next available place
			for (int col = 0; col < CITY_BLOCKS_COLS; col++) {
				if (blocksL[col] == 0) {

					int requestedCols = (int)ceilf(ofRandom(0,CITY_BLOCKS_ROWS));
					int requestedRows = (int)ceilf(ofRandom(0,CITY_BLOCKS_ROWS));

					float height = ofRandom(100, blockProbability * 2000.);

					for (int i = col; i < ofClamp(CITY_BLOCKS_COLS + requestedCols, col, CITY_BLOCKS_COLS); i++) {						
						for (int j = 0; j < ofClamp(CITY_BLOCKS_ROWS + requestedRows, 0, CITY_BLOCKS_ROWS); j++) {
							blocksL[j * CITY_BLOCKS_COLS + i] = height;
						}
					}


					break;

				}
			}
		}

	}
}

//--------------------------------------------------------------
void ofxCity::update(){
	float hash = roadWidth * 2. 
		+ roadHeight * 3. 
		+ roadLineWidth * 5.
		+ roadLineHeight * 7.
		+ roadTexWidth * 11.
		+ roadTexHeight * 13.;
	if (hash != roadParamsHash)
	{
		setupTextures();

		roadParamsHash = hash;
	}

	updateRoad();

	curSpeed = tween.update();
	curDistance += curSpeed;
	curDistanceOffset += curSpeed;

	updateBlocks();
}

//--------------------------------------------------------------
void ofxCity::draw(){


	ofEnableAlphaBlending();
	ofBackground(255,255,255,255);
	ofPushMatrix();
	texRoad.bind();

	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	ofTranslate(0, 20);

	ofTranslate(0, 0, curDistance + 650);
	//for(std::vector<ofPlanePrimitive>::iterator planeIt = roads.begin(); planeIt != roads.end(); ++planeIt) {
	//ofPlanePrimitive plane = *planeIt;
	for(auto & plane: roads) {

		ofSetColor(255);	


		if (bWireframe) {
			plane.drawWireframe();
		} else {
			plane.draw();
		}
	}


	texRoad.unbind();

	//ofFill();
	ofDrawAxis(50);
	ofDisableAlphaBlending();


	ofPopMatrix();

	ofApp *app = (ofApp *)ofxGetAppPtr();
	app->cam.end();
	if (bShowGui) {
		ofDisableDepthTest();
		gui.draw();
	}
	app->cam.begin();

}

//--------------------------------------------------------------
void ofxCity::keyPressed(int key){
	ofApp *app = (ofApp *)ofxGetAppPtr();

	switch (key)
	{
	case 'r': setup(); break;
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
		blockProbability = ofClamp(blockProbability+0.1,0,1);
		generateBlocks(); 
		break;
	default:
		break;
	}
}