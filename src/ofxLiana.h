#pragma once

#include "ofMain.h"
#include "ofxSpring.h"
#include <limits>


class ofxLiana
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
	
	
	float xPosition;
	bool lianaMode; // if true, nodes are placed at random
	// if false, nodes X are all at xPosition
	bool lockLastNode;
	
	// ------ constructors ------
	
	ofxLiana(float theX);
	
	ofxLiana();
	~ofxLiana();
	
	void setup();
	void draw();
	void update(bool theLockX, bool theLockY, bool theLockZ);
	void initNodesAndSprings();
	void initNodesAndSprings_Liana();
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
