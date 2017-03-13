//
//  ofxLiana.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 13/03/2017.
//
//

#include "ofxLiana.h"


// implementation
// ------ constructors ------

ofxLiana::ofxLiana(float theX) :
	xPosition(theX)
	,lianaMode(true)
	, lockLastNode(false)
{
}

ofxLiana::ofxLiana() : lianaMode(false)
{
}
ofxLiana::~ofxLiana() {
	cleanUp();
}
void ofxLiana::cleanUp() {
	for (int i = 0; i < springs.size(); i++) {
		delete springs[i];
	}
	springs.clear();
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
	nodes.clear();
}
//--------------------------------------------------------------
void ofxLiana::initNodesAndSprings() {
	
	bRepulse = false;
	dragNodeIndex = -1;
	cleanUp();
	
	// init nodes
	int width = ofGetWidth();
	int height = ofGetHeight();
	float rad = nodeDiameter / 2.f;
	for (int i = 0; i < numNodes; i++) {
		ofxNode *node = new ofxNode(width / 2 + ofRandom(-200, 200), height / 2 + ofRandom(-200, 200), false);
		// use this for 3D
		//node->setBoundary(rad, rad, rad, width - rad, height - rad, width - rad);
		node->setBoundary(rad, rad, width - rad, height - rad);
		node->radius = nodeRadius;
		node->strength = nodeStrength;
		node->damping = nodeDamping;
		node->ramp = nodeRamp;
		node->maxVelocity = nodeVelocity;
		node->id = i;
		nodes.push_back(node);
	}
	
	// set springs randomly
	
	for (int j = 0; j < nodes.size() - 1; j++) {
		int rCount = floor(ofRandom(1, 2));
		for (int i = 0; i < rCount; i++) {
			int r = floor(ofRandom(j + 1, nodes.size()));
			ofxSpring *newSpring = new ofxSpring(*nodes[j], *nodes[r]);
			newSpring->length = springLength;
			newSpring->stiffness = springStiffness;
			newSpring->damping = stringDamping;
			newSpring->id = j;
			springs.push_back(newSpring);
		}
	}
}
//--------------------------------------------------------------
void ofxLiana::initNodesAndSprings_Liana() {
	
	bRepulse = false;
	dragNodeIndex = -1;
	cleanUp();
	
	// init nodes
	int width = ofGetWidth();
	int height = ofGetHeight();
	float spacing = height / (numNodes - 1);
	float rad = nodeDiameter / 2.f;
	for (int i = 0; i < numNodes; i++) {
		bool isLocked = (i == 0) || (lockLastNode && (i == (numNodes - 1)));
		float xPos = xPosition
		+ (isLocked ? 0
		   : ofRandom(-50, 50));
		ofxNode *node = new ofxNode(xPos, spacing * i, isLocked);
		// use this for 3D
		//node->setBoundary(rad, rad, rad, width - rad, height - rad, width - rad);
		node->setBoundary(rad, rad, width - rad, height - rad);
		node->radius = nodeRadius;
		node->strength = nodeStrength;
		node->damping = nodeDamping;
		node->ramp = nodeRamp;
		node->maxVelocity = nodeVelocity;
		node->id = i;
		nodes.push_back(node);
	}
	
	// set springs
	for (int i = 0; i < numNodes - 1; i++) {
		ofxSpring *newSpring = new ofxSpring(*nodes[i], *nodes[i + 1]);
		newSpring->length = springLength;
		newSpring->stiffness = springStiffness;
		newSpring->damping = stringDamping;
		newSpring->id = i;
		springs.push_back(newSpring);
	}
	
}
void ofxLiana::setup() {
	bRepulse = false;
	repulsionCenters.clear();
	if (!lianaMode) {
		initNodesAndSprings();
	}
	else {
		initNodesAndSprings_Liana();
	}
}
void ofxLiana::update(bool theLockX, bool theLockY, bool theLockZ) {
	
	// let all nodes repel each other
	for (int i = 0; i < nodes.size(); i++) {
		if (bRepulse) {
			for (auto & rep : repulsionCenters)
			{
				nodes[i]->attract(nodes, rep, repulsionStrength, repulsionRadius);
			}
		}
		else {
			nodes[i]->attract(nodes);
		}
	}
	// apply spring forces
	for (int i = 0; i < springs.size(); i++) {
		springs[i]->update();
	}
	// apply velocity vector and update position
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->update(gravity, theLockX, theLockY, theLockZ);
	}
	
	if (dragNodeIndex > -1) {
		nodes[dragNodeIndex]->setPosition(ofGetMouseX(), ofGetMouseY());
	}
}
void ofxLiana::draw() {
	
	ofSetColor(ofColor::white);
	//ofSetColor(0, 130, 164);
	ofSetLineWidth(lineWidth);
	for (int i = 0; i < springs.size(); i++) {
		ofLine(springs[i]->fromNode->x, springs[i]->fromNode->y, springs[i]->fromNode->z, springs[i]->toNode->x, springs[i]->toNode->y, springs[i]->toNode->z);
	}
	// draw nodes
	//noStroke();
	for (int i = 0; i < nodes.size(); i++) {
		ofFill();
		ofEllipse(nodes[i]->x, nodes[i]->y, nodes[i]->z, nodeDiameter, nodeDiameter);
		/*ofSetColor(0);
		 ofEllipse(nodes[i]->x, nodes[i]->y, nodeDiameter - 4, nodeDiameter - 4);*/
	}
}
//--------------------------------------------------------------
void ofxLiana::keyPressed(int key) {
	switch (key) {
		case ' ': {
			bRepulse = true;
			
		}break;
	}
}
//--------------------------------------------------------------
void ofxLiana::keyReleased(int key) {
	switch (key) {
		case ' ': bRepulse = false; break;
	}
}
//--------------------------------------------------------------
void ofxLiana::mousePressed(int x, int y, int button) {
	// Ignore anything greater than this distance
	float maxDist = 20;
	for (int i = 0; i < nodes.size(); i++) {
		ofxNode *checkNode = nodes[i];
		float d = ofVec2f(x, y).distance(ofVec3f(checkNode->x, checkNode->y));
		if (d < maxDist) {
			dragNodeIndex = i;
			maxDist = d;
		}
	}
	
	/*if (bRepulse) {
		repulsionCenter = ofVec3f(ofGetMouseX(), ofGetMouseY());
	 }*/
}
//--------------------------------------------------------------
void ofxLiana::mouseMoved(int x, int y) {
	/*if (bRepulse) {
		repulsionCenter = ofVec3f(x, y);
	 }*/
}
//--------------------------------------------------------------
void ofxLiana::clearRepulsors() {
	repulsionCenters.clear();
}
//--------------------------------------------------------------
void ofxLiana::addRepulsor(int x, int y) {
	if (bRepulse) {
		repulsionCenters.push_back(ofVec3f(x, y));
	}
}

//--------------------------------------------------------------
void ofxLiana::mouseReleased(int x, int y, int button) {
	if (dragNodeIndex > -1) {
		dragNodeIndex = -1;
	}
	
}

