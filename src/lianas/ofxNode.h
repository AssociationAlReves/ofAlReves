#pragma once

#include "ofMain.h"
#include <limits>

#define OFXNODE_DAMPING 0.1f
#define OFXNODE_VELOCITY 0.1
#define OFXNODE_RADIUS 200
#define OFXNODE_STRENGTH -1
#define OFXNODE_RAMP 1.0f

class ofxNode : public ofVec3f
{
public:


	// ------ public properties ------
	// if needed, an ID for the node
	int id;

	/**
	* Minimum x position
	*/
	float minX;
	/**
	* Maximum x position
	*/
	float maxX;
	/**
	* Minimum y position
	*/
	float minY;
	/**
	* Maximum y position
	*/
	float maxY;
	/**
	* Minimum z position
	*/
	float minZ;
	/**
	* Maximum z position
	*/
	float maxZ;

	bool locked; // if true, node can't move (liana top node)
				 /**
				 * Velocity vector (speed)
				 */
	ofVec3f *velocity;
	ofVec3f previousVelocity;
	ofVec3f repulsionForce; // test: no subject to X/Y/Z locks
							/**
							* Maximum length of the velocity vector (default = 10)
							*/
	float maxVelocity;
	/**
	* Damping of the velocity: 0=no damping, 1=full damping (default = 0.5)
	*/
	float damping;
	// inertia of node: 0=no inertia (not used for now)
	// public float inertia = 0.5f;

	// radius of impact
	float radius;
	// strength: positive for attraction, negative for repulsion (default for
	// Nodes)
	float strength;
	// parameter that influences the form of the function
	float ramp;

	// ------ calculate attraction ------
	void attract(vector<ofxNode*>& theNodes);
	void attract(vector<ofxNode*>& theNodes, const ofVec3f& repulsionCenter, float repulsionStrength, float repulsionRadius);

	void attract(ofxNode* theNode);

	void setPosition(float theX, float theY);

	// ------ constructors ------

	ofxNode(float theX, float theY, bool isLocked);

	void update(float gravity, bool theLockX, bool theLockY, bool theLockZ);
	void update(bool theLockX, bool theLockY, bool theLockZ);

	void setBoundary(const float theMinX, const float theMinY, const float theMinZ,
		const float theMaxX, const float theMaxY, const float theMaxZ);
	void setBoundary(const float theMinX, const float theMinY, const float theMaxX,
		const float theMaxY);

};

