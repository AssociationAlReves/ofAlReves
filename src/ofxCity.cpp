#include "ofxCity.h"
#include "ofApp.h"

//--------------------------------------------------------------
void ofxCity::setup(){

	gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
    gui.add(bWireframe.set("Wireframe", false));	
    bShowGui = false;

	fboRoad.allocate(100,100);
	fboRoad.begin();

	ofBackground(ofColor::darkSlateGray);
	ofSetColor(ofColor::white);
	ofFill();
	ofRect(47,25,0,6,50);

	fboRoad.end();

	texRoad = fboRoad.getTextureReference();
}

//--------------------------------------------------------------
void ofxCity::update(){

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