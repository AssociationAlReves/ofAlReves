#include "ofxCity.h"
#include "ofApp.h"

//--------------------------------------------------------------
void ofxCity::setup(){

	roadParamsHash = 0;
	gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
	gui.add(bWireframe.set("Wireframe", false));
	roadParams.setName("Road params");
	roadParams.add(roadTexWidth.set("Road tex width", 100,10,1000));
	roadParams.add(roadTexHeight.set("Road tex height", 100,10,1000));		
	roadParams.add(roadLineWidth.set("Road line width %", 10,1,100));
	roadParams.add(roadLineHeight.set("Road line height %", 50,1,100));	
	roadParams.add(roadWidth.set("Road width", 100,10,1000));
	roadParams.add(roadHeight.set("Road height", 100,10,1000));

	gui.add(roadParams);

	bShowGui = false;

	setupTextures();
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
void ofxCity::update(){
	int hash = roadWidth ^ roadHeight ^ roadLineWidth ^ roadLineHeight ^ roadTexWidth ^ roadTexHeight;
	if (hash != roadParamsHash)
	{
		setupTextures();

		roadParamsHash = hash;
	}
}

//--------------------------------------------------------------
void ofxCity::draw(){
	
	ofEnableAlphaBlending();
	ofBackground(255,255,255,255);

	//ofNoFill();
	//ofSetColor(ofColor::red, 255);
	//fboRoad.draw(500,500,100,100);
	ofSetColor(255);
	plane.set(100,100);
	plane.setResolution(3,3);
	plane.resizeToTexture( texRoad );
	plane.setPosition(100,100, 0);
	texRoad.bind();

	if (bWireframe)
	{
		plane.drawWireframe();
	}
	else
	{
		plane.draw();
	}

	texRoad.unbind();

	//ofFill();

	ofDisableAlphaBlending();

	if (bShowGui) {
		ofDisableDepthTest();
		gui.draw();
	}
}

//--------------------------------------------------------------
void ofxCity::keyPressed(int key){
	ofApp *app = (ofApp *)ofxGetAppPtr();

	switch (key)
	{
	case 'r': setup(); break;
	case 'h': {
		bShowGui = !bShowGui;
		if (bShowGui){
			app->cam.disableMouseInput();
		}
		else {
			app->cam.enableMouseInput();
		}
			  }
	default:
		break;
	}
}