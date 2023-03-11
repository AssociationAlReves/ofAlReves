//
//  ofxSpring.cpp
//  ofAlReves
//
//  Created by Xavier Fischer on 13/03/2017.
//
//

#include "ofxSpring.h"


ofxSpring::ofxSpring(ofxNode& theFromNode, ofxNode& theToNode) :
length(100), stiffness(0.6f), damping(0.9f), fromNode(&theFromNode), toNode(&theToNode)
{
}
ofxSpring::ofxSpring(ofxNode& theFromNode, ofxNode& theToNode, float theLength,
							float theStiffness, float theDamping) :
length(theLength), stiffness(theStiffness), damping(theDamping), fromNode(&theFromNode), toNode(&theToNode)
{
}

void ofxSpring::update() {
	// calculate the target position
	// target = normalize(to - from) * length + from
	ofVec3f diff = (ofVec3f)*toNode - (ofVec3f)*fromNode;
	diff = length * diff.normalize();
	ofVec3f target = (ofVec3f)*fromNode + diff;
	
	ofVec3f force = target - (ofVec3f)*fromNode;
	
	
	if (fromNode->locked && !toNode->locked) {
		//force *= 0.5f;
		force *= (1 - damping);
		force *= stiffness;
		
		//*(fromNode->velocity) += force;
		*(toNode->velocity) += (-1 * force);
	}
	else if (toNode->locked && !fromNode->locked) {
		//force *= 0.5f;
		
		force *= (1 - damping);
		force *= stiffness;
		*(fromNode->velocity) += force;
		//*(toNode->velocity) += -1 * force;
	}
	else if (!toNode->locked && !fromNode->locked) {
		force *= 0.5f;
		force *= (1 - damping);
		force *= stiffness;
		*(fromNode->velocity) += force;
		*(toNode->velocity) += (-1 * force);
	}
	
	
	
	
	/*(*fromNode)->velocity += force;
	 (*fromNode)->velocity += -1 * force;*/
}