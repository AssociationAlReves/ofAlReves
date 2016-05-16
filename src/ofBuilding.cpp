//
//  ofBuilding.cpp
//
//  Created by Xavier Fischer on 06/05/2016.
//
//

#include "ofMain.h"
#include "ofBuilding.h"


//--------------------------------------------------------------
void ofBuilding::setup(){



}

void ofBuilding::update(){


}

void ofBuilding::draw(float rotateAngle){
	ofPushMatrix();

	ofVec3f pos = box.getPosition();
	ofTranslate(-pos.x, -pos.y, -pos.z);
	ofRotate(rotateAngle,-1,0,0);
	ofTranslate(pos.x, pos.y, pos.z);
	
	box.draw();

	ofPopMatrix();
}

void ofBuilding::drawWireframe(float rotateAngle, float margin){
		
	box.setDepth(box.getDepth()+10);
	box.setWidth(box.getWidth()+10);
	box.drawWireframe();
	box.setDepth(box.getDepth()-10);
	box.setWidth(box.getWidth()-10);

}