#pragma once

#include "ofMain.h"
#include <limits>

class ofxNode
{
public:

	ofVec3f position;

	// ------ public properties ------
	// if needed, an ID for the node
	int id;

	/**
	* Diameter of the node (useful if a click on the node has to be checked)
	*/
	float diameter;

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

	/**
	* Velocity vector (speed)
	*/
	ofVec3f velocity;
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
	void attract(vector<ofxNode>& theNodes);

	void attract(ofxNode& theNode);

	// ------ constructors ------
	
	ofxNode(float theX, float theY);

	ofxNode(float theX, float theY, float theZ);

	ofxNode(const ofxNode& theVector);

	ofxNode();

	void update();
	void update(bool theLockX, bool theLockY, bool theLockZ);

	void setBoundary(const float theMinX, const float theMinY, const float theMinZ,
		const float theMaxX, const float theMaxY, const float theMaxZ);
	void setBoundary(const float theMinX, const float theMinY, const float theMaxX,
		const float theMaxY);

};


// ------ constructors ------

inline ofxNode::ofxNode(float theX, float theY) : position(theX, theY)
, minX(std::numeric_limits<float>::min())
, maxX(std::numeric_limits<float>::max())
, minY(std::numeric_limits<float>::min())
, maxY(std::numeric_limits<float>::max())
, minZ(std::numeric_limits<float>::min())
, maxZ(std::numeric_limits<float>::max())
, velocity(ofVec3f(0))
, maxVelocity(10)
, damping(0.5f)
, radius(200)
, strength(-1)
, ramp(1.0) {}

inline ofxNode::ofxNode(float theX, float theY, float theZ) : position(theX, theY, theZ)
, minX(std::numeric_limits<float>::min())
, maxX(std::numeric_limits<float>::max())
, minY(std::numeric_limits<float>::min())
, maxY(std::numeric_limits<float>::max())
, minZ(std::numeric_limits<float>::min())
, maxZ(std::numeric_limits<float>::max())
, velocity(ofVec3f(0))
, maxVelocity(10)
, damping(0.5f)
, radius(200)
, strength(-1)
, ramp(1.0)
{	}

inline ofxNode::ofxNode(const ofxNode& theVector) : position(theVector.position.x, theVector.position.y, theVector.position.z)
, minX(std::numeric_limits<float>::min())
, maxX(std::numeric_limits<float>::max())
, minY(std::numeric_limits<float>::min())
, maxY(std::numeric_limits<float>::max())
, minZ(std::numeric_limits<float>::min())
, maxZ(std::numeric_limits<float>::max())
, velocity(ofVec3f(0))
, maxVelocity(10)
, damping(0.5f)
, radius(200)
, strength(-1)
, ramp(1.0) {}

inline ofxNode::ofxNode() : position(0)
, minX(std::numeric_limits<float>::min())
, maxX(std::numeric_limits<float>::max())
, minY(std::numeric_limits<float>::min())
, maxY(std::numeric_limits<float>::max())
, minZ(std::numeric_limits<float>::min())
, maxZ(std::numeric_limits<float>::max())
, velocity(ofVec3f(0))
, maxVelocity(10)
, damping(0.5f)
, radius(200)
, strength(-1)
, ramp(1.0) {}


// ---------------- implementation
// ------ calculate attraction ------
inline void ofxNode::attract(vector<ofxNode> & theNodes) {
	// attraction or repulsion part
	for (int i = 0; i< theNodes.size(); i++) {
		ofxNode otherNode = theNodes[i];
		//// stop when empty
		//if (otherNode == NULL)
		//	break;
		// not with itself
		if (otherNode.id == id)
			continue;

		this->attract(otherNode);
	}
}

inline void ofxNode::attract(ofxNode& theNode) {
	float d = this->position.distance(theNode.position);

	if (d > 0 && d < radius) {
	float s = pow(d / radius, 1 / ramp);
	float f = s * 9 * strength * (1 / (s + 1) + ((s - 3) / 4)) / d;
	ofVec3f df = this->position - theNode.position;
	df *= f;

	theNode.velocity.x += df.x;
	theNode.velocity.y += df.y;
	theNode.velocity.z += df.z;
	}
}



inline void ofxNode::update()
{
	update(false, false, false);
}
inline void ofxNode::update(bool theLockX, bool theLockY, bool theLockZ)
{

	velocity.limit(maxVelocity);

	// prevent oscillating by reducing velocity if angle to previous
	// velocity is very large
	/*float da = PVector.angleBetween(velocity, pVelocity);
	if (!Float.isNaN(da)) {
	da = PApplet.abs(1 - (da / PApplet.PI));
	//da = PApplet.pow(da, 4);
	if (da < 0.5) da = 0;
	PApplet.println(id + ", " + da);
	velocity.mult(da);
	}*/
	if (!theLockX) position.x += velocity.x;
	if (!theLockY) position.y += velocity.y;
	if (!theLockZ) position.z += velocity.z;

	if (position.x < minX) {
		position.x = minX - (position.x - minX);
		velocity.x = -velocity.x;
	}
	if (position.x > maxX) {
		position.x = maxX - (position.x - maxX);
		velocity.x = -velocity.x;
	}

	if (position.y < minY) {
		position.y = minY - (position.y - minY);
		velocity.y = -velocity.y;
	}
	if (position.y > maxY) {
		position.y = maxY - (position.y - maxY);
		velocity.y = -velocity.y;
	}

	if (position.z < minZ) {
		position.z = minZ - (position.z - minZ);
		velocity.z = -velocity.z;
	}
	if (position.z > maxZ) {
		position.z = maxZ - (position.z - maxZ);
		velocity.z = -velocity.z;
	}

	// x = PApplet.constrain(x, minX, maxX);
	// y = PApplet.constrain(y, minY, maxY);
	// z = PApplet.constrain(z, minZ, maxZ);

	velocity *= (1 - damping);
}

inline void ofxNode::setBoundary(float theMinX, float theMinY, float theMinZ,
	float theMaxX, float theMaxY, float theMaxZ) {
	minX = theMinX;
	maxX = theMaxX;
	minY = theMinY;
	maxY = theMaxY;
	minZ = theMinZ;
	maxZ = theMaxZ;
}

inline void ofxNode::setBoundary(float theMinX, float theMinY, float theMaxX,
	float theMaxY) {
	minX = theMinX;
	maxX = theMaxX;
	minY = theMinY;
	maxY = theMaxY;
}