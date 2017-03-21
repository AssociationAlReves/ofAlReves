//
//  ofxLianaMesh.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 13/03/2017.
//
//

#include "ofxLianaMesh.h"



// implementation
// ------ constructors ------

ofxLianaMesh::ofxLianaMesh() : lockLastNode(false)
{
}

ofxLianaMesh::~ofxLianaMesh() {
	cleanUp();
}
void ofxLianaMesh::cleanUp() {
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
void ofxLianaMesh::initNodesAndSprings_Mesh() {
	
	bRepulse = false;
	dragNodeIndex = -1;
	cleanUp();
	
	// init nodes
	int width = ofGetWidth();
	int height = ofGetHeight();
	float xSpacing = width / (numNodes - 1);
	float ySpacing = height / (numNodes - 1);
	float rad = nodeDiameter / 2.f;
	int idNode = 0;
	for (int i = 0; i < numNodes; i++) {
		
		float xPos = i * xSpacing;
		
		for (int j = 0; j < numNodes; j++) {
			
			bool isLocked = (i == 0)
							|| (j == 0)
							|| (lockLastNode && (i == (numNodes - 1)))
							|| (lockLastNode && (j == (numNodes - 1)));
			
			
			float yPos = j * ySpacing;
//			float xPos = xPosition
//			+ (isLocked ? 0
//			   : ofRandom(-50, 50));
			ofxNode *node = new ofxNode(xPos, yPos, isLocked);
			// use this for 3D
			//node->setBoundary(rad, rad, rad, width - rad, height - rad, width - rad);
			node->setBoundary(rad, rad, width - rad, height - rad);
			node->radius = nodeRadius;
			node->strength = nodeStrength;
			node->damping = nodeDamping;
			node->ramp = nodeRamp;
			node->maxVelocity = nodeVelocity;
			node->id = idNode++;
			nodes.push_back(node);
			
		}
	}
	
	int id = 0;
	for (int i = 0; i < numNodes; i++) {
		for (int j = 0; j < numNodes; j++) {
			
			if (j != 0) {
				// Vertical
				ofxSpring *newSpring = new ofxSpring(*nodes[i*numNodes+j], *nodes[i * numNodes + (j - 1)]);
				newSpring->length = springLength;
				newSpring->stiffness = springStiffness;
				newSpring->damping = stringDamping;
				newSpring->id = id++;
				springs.push_back(newSpring);
			}
			
			if (i != (numNodes-1)) {
				// Horizontal
				ofxSpring *newSpring = new ofxSpring(*nodes[i*numNodes+j], *nodes[(i+1) * numNodes + j]);
				newSpring->length = springLength;
				newSpring->stiffness = springStiffness;
				newSpring->damping = stringDamping;
				newSpring->id = id++;
				springs.push_back(newSpring);
			}
			
		}
	}
	
	
	
}
void ofxLianaMesh::setup() {
	bRepulse = false;
	repulsionCenters.clear();
	
	initNodesAndSprings_Mesh();
	
}
void ofxLianaMesh::update(bool theLockX, bool theLockY, bool theLockZ) {
	//clearRepulsors();
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
void ofxLianaMesh::draw() {
	
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
void ofxLianaMesh::keyPressed(int key) {
	switch (key) {
		case ' ': {
			bRepulse = true;
			
		}break;
	}
}
//--------------------------------------------------------------
void ofxLianaMesh::keyReleased(int key) {
	switch (key) {
		case ' ': bRepulse = false; break;
	}
}
//--------------------------------------------------------------
void ofxLianaMesh::mousePressed(int x, int y, int button) {
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
void ofxLianaMesh::mouseMoved(int x, int y) {
	/*if (bRepulse) {
		repulsionCenter = ofVec3f(x, y);
	 }*/
}
//--------------------------------------------------------------
void ofxLianaMesh::clearRepulsors() {
	repulsionCenters.clear();
}
//--------------------------------------------------------------
void ofxLianaMesh::addRepulsor(int x, int y) {
	if (bRepulse) {
		repulsionCenters.push_back(ofVec3f(x, y));
		cout << repulsionCenters.size() << endl;
	}
}

//--------------------------------------------------------------
void ofxLianaMesh::mouseReleased(int x, int y, int button) {
	if (dragNodeIndex > -1) {
		dragNodeIndex = -1;
	}
	
}

