//
//  ofBuilding.cpp
//
//  Created by Xavier Fischer on 06/05/2016.
//
//

#include "ofMain.h"
#include "ofBuilding.h"


//--------------------------------------------------------------
void ofBuilding::setup() {



}

void ofBuilding::update() {


}

void ofBuilding::draw(float rotateAngle, float translation, float alpha, bool wireFrame, float margin) {


	ofPushMatrix();

	if (rotateAngle == 0) {
		ofTranslate(position);
		ofTranslate(0, 0, translation);
	}
	else
	{
		ofTranslate(position);
		ofTranslate(0, 0, translation);

		// translate to box base front side center
		ofTranslate(0, box.getHeight() / 2, box.getDepth() / 2);

		ofRotate(rotateAngle, -1, 0, 0);



		ofTranslate(0, -box.getHeight() / 2, -box.getDepth() / 2);
	}



	ofFill();
	ofSetColor(255);

	ofSetColor(ofColor::white, alpha);

	box.draw();

	if (wireFrame) {
		ofNoFill();
		ofSetColor(0);
		ofSetLineWidth(2);

		ofSetColor(ofColor::white, alpha);

		box.setDepth(box.getDepth() + 10);
		box.setWidth(box.getWidth() + 10);
		box.drawWireframe();
		box.setDepth(box.getDepth() - 10);
		box.setWidth(box.getWidth() - 10);

		ofFill();
		ofSetColor(255);
		ofSetLineWidth(1);

	}

	ofPopMatrix();
}
