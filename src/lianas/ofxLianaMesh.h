//
//  ofxLianaMesh.h
//  ofAlReves
//
//  Created by Xavier Fischer on 13/03/2017.
//
//

#ifndef ofxLianaMesh_h
#define ofxLianaMesh_h

#include <stdio.h>

#endif /* ofxLianaMesh_h */

#pragma once

#include "ofMain.h"
#include "ofxSpring.h"
#include <limits>


class ofxLianaMesh
{
public:
	
	ofxSpring *spring;
	
	// ----- parameters ---------
	int numNodes;
	float nodeRadius;
	float nodeStrength;
	float nodeDiameter;
	float nodeDamping;
	float nodeRamp;
	float nodeVelocity;
	
	float springLength;
	float springStiffness;
	float stringDamping;
	
	
	float gravity;
	float lineWidth;
	
	
	float repulsionRadius;
	float repulsionStrength;
	bool bRepulse;
	vector<ofVec3f> repulsionCenters;
	int dragNodeIndex;
	
	bool lockLastNode;
	
	// ------ constructors ------
	
	ofxLianaMesh();
	~ofxLianaMesh();
	
	void setup();
	void draw();
	void update(bool theLockX, bool theLockY, bool theLockZ);
	void initNodesAndSprings_Mesh();
	void cleanUp();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void clearRepulsors();
	void addRepulsor(int x, int y);
	void mouseMoved(int x, int y);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
		// an array for the nodes
	vector<ofxNode*> nodes;// = new Node[100];
	// an array for the springs
	vector<ofxSpring*> springs; // = new Spring[0];
	
	// dragged node
	
};

