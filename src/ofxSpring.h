#pragma once

#include "ofMain.h"
#include "ofxNode.h"
#include <limits>

/**
* Elasic connection between two nodes.
*/
class ofxSpring
{
public:

	// ------ public properties ------
	// if needed, an ID for the node
	int id;

	ofxNode* fromNode;
	ofxNode* toNode;

	float length;// = 100;
	float stiffness;// = 0.6f;
	float damping;// = 0.9f;

				  // ------ constructors ------
				  /**
				  * @param theFromNode
				  *            Node from which the spring starts
				  * @param theToNode
				  *            Node to which the spring goes
				  */
	ofxSpring(ofxNode& theFromNode, ofxNode& theToNode);
	ofxSpring(ofxNode& theFromNode, ofxNode& theToNode, float theLength,
		float theStiffness, float theDamping);

	void update();
};
