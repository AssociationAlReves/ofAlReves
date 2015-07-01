#include "ofxRibbon.h"


//--------------------------------------------------------------
void ofxRibbon::setup(){
	sWidth = ofGetWidth();
	sHeight = ofGetHeight();

	points.clear();
	endLine = false;
}

//--------------------------------------------------------------
void ofxRibbon::update(){

	if (endLine && tween.isCompleted())
	{
		points.clear();
	}

	ofVec3f sumOfAllPoints(0,0,0);
	for(unsigned int i = 0; i < points.size(); i++){
		points[i].z -= 4;
		sumOfAllPoints += points[i];
	}
	center = sumOfAllPoints / points.size();


}

//--------------------------------------------------------------
void ofxRibbon::clear(){

	endLine = true;
	tween.setParameters(easinglinear, ofxTween::easeInOut, 1, 0, 5000, 0);
}

//--------------------------------------------------------------
void ofxRibbon::draw(){

	float tweenFactor = 1;
	if (endLine)
	{
		tweenFactor = tween.update();
	}

	int w = ofGetWidth();
	int h = ofGetHeight();

	ofApp *app = (ofApp *)ofxGetAppPtr();
	ofEasyCam cam = app->cam;

	ofSetColor(0);
	//do the same thing from the first example...
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	for(unsigned int i = 1; i < points.size(); i++){

		//find this point and the next point
		ofVec3f thisPoint = points[i-1];
		ofVec3f nextPoint = points[i];

		//get the direction from one to the next.
		//the ribbon should fan out from this direction
		ofVec3f direction = (nextPoint - thisPoint);

		//get the distance from one point to the next
		float distance = direction.length();

		//get the normalized direction. normalized vectors always have a length of one
		//and are really useful for representing directions as opposed to something with length
		ofVec3f unitDirection = direction.normalized();

		//find both directions to the left and to the right
		ofVec3f toTheLeft = unitDirection.getRotated(-90, ofVec3f(0,0,1));
		ofVec3f toTheRight = unitDirection.getRotated(90, ofVec3f(0,0,1));

		//use the map function to determine the distance.
		//the longer the distance, the narrower the line.
		//this makes it look a bit like brush strokes
		float thickness = ofMap(distance, 0, 60, 20, 2, true);
		thickness *= tweenFactor;

		//calculate the points to the left and to the right
		//by extending the current point in the direction of left/right by the length
		ofVec3f leftPoint = thisPoint+toTheLeft*thickness;
		ofVec3f rightPoint = thisPoint+toTheRight*thickness;

		//add these points to the triangle strip
		mesh.addVertex(ofVec3f(leftPoint.x, leftPoint.y, leftPoint.z));
		mesh.addVertex(ofVec3f(rightPoint.x, rightPoint.y, rightPoint.z));

		float noiseR = ofNoise(ofGetElapsedTimef()*0.5+i*0.123);
		float noiseG = 0; //0.3 * ofNoise(ofGetElapsedTimef()*0.5+i*0.456);
		float noiseB = 0; //0.3 * ofNoise(ofGetElapsedTimef()*0.5+i*0.789);		
		mesh.addColor(ofFloatColor(noiseR,noiseG,noiseB,0.01));
		mesh.addColor(ofFloatColor(noiseR,noiseG,noiseB,0.01));
	}

	//end the shape
	mesh.draw();
}

//--------------------------------------------------------------
void ofxRibbon::mouseMoved(int x, int y ){
	////if we are using the camera, the mouse moving should rotate it around the whole sculpture
	//   if(usecamera){
	//       float rotateAmount = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 360);
	//       ofVec3f furthestPoint;
	//       if (points.size() > 0) {
	//           furthestPoint = points[0];
	//       }
	//       else
	//       {
	//           furthestPoint = ofVec3f(x, y, 0);
	//       }

	//       ofVec3f directionToFurthestPoint = (furthestPoint - center);
	//       ofVec3f directionToFurthestPointRotated = directionToFurthestPoint.rotated(rotateAmount, ofVec3f(0,1,0));
	//       camera.setPosition(center + directionToFurthestPointRotated);
	//       camera.lookAt(center);
	//   }
	////otherwise add points like before
	//   else{
	//       ofVec3f mousePoint(x,y,0);
	//       points.push_back(mousePoint);
	//   }
	ofVec3f mousePoint(x,y,0);
	points.push_back(mousePoint);
}



//--------------------------------------------------------------
void ofxRibbon::keyPressed(int key){
	switch (key)
	{
	case ' ' : clear(); break;
	case 'r': setup(); break;
	default:
		break;
	}
}