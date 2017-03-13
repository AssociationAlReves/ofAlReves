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

inline ofxSpring::ofxSpring(ofxNode& theFromNode, ofxNode& theToNode) :
	length(100), stiffness(0.6f), damping(0.9f), fromNode(&theFromNode), toNode(&theToNode)
{
}
inline ofxSpring::ofxSpring(ofxNode& theFromNode, ofxNode& theToNode, float theLength,
	float theStiffness, float theDamping) :
	length(theLength), stiffness(theStiffness), damping(theDamping), fromNode(&theFromNode), toNode(&theToNode)
{
}

inline void ofxSpring::update() {
	// calculate the target position
	// target = normalize(to - from) * length + from
	ofVec3f diff = (ofVec3f)*toNode - (ofVec3f)*fromNode;
	diff = length * diff.normalize();
	ofVec3f target = (ofVec3f)*fromNode + diff;

	ofVec3f force = target - (ofVec3f)*fromNode;


	if (fromNode->locked) {
		//force *= 0.5f;
		force *= (1 - damping);
		force *= stiffness;

		//*(fromNode->velocity) += force;
		*(toNode->velocity) += (-1 * force);
	}
	else if (toNode->locked) {
		//force *= 0.5f;

		force *= (1 - damping);
		force *= stiffness;
		*(fromNode->velocity) += force;
		//*(toNode->velocity) += -1 * force;
	}
	else {
		force *= 0.5f;
		force *= (1 - damping);
		force *= stiffness;
		*(fromNode->velocity) += force;
		*(toNode->velocity) += (-1 * force);
	}




	/*(*fromNode)->velocity += force;
	(*fromNode)->velocity += -1 * force;*/
}