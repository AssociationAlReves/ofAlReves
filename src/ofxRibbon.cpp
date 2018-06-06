#include "ofxRibbon.h"
#include "ofApp.h"


//--------------------------------------------------------------
void ofxRibbon::setup(){
	sWidth = ofGetWidth();
	sHeight = ofGetHeight();

	points.clear();
	endLine = false;
	maxZ = 0;

    if (bGuiLoaded == false){
        gui.setup("Ribbon control", Globals::hostName + RIBBON_SETTINGS_FILE);
        
        gui.add(speedParam.set("speed",2,0,20));
        gui.loadFromFile(Globals::hostName + RIBBON_SETTINGS_FILE);
        
        bGuiLoaded = true;
    }

}

//--------------------------------------------------------------
void ofxRibbon::update(){

	float tweenFactor = 1;
	bool isTweenCompleted = ofGetElapsedTimef() > tweenInTo;
	if (endLine) {
		if ( isTweenCompleted) {
			points.clear();
			maxZ = 0;
		}
	}


	ofVec3f sumOfAllPoints(0,0,0);
	for(unsigned int i = 0; i < points.size(); i++){
        if (!endLine){
            points[i].z -= speedParam;
        }
		maxZ = min(maxZ,points[i].z);
		sumOfAllPoints += points[i];
	}
	center = sumOfAllPoints / points.size();


}

//--------------------------------------------------------------
void ofxRibbon::clear(){

	endLine = true;
	tweenInFrom = ofGetElapsedTimef();
	tweenInTo = tweenInFrom + 4;
	ofVec3f dest  = ofVec3f(0,0,maxZ + 200);
	//ofApp *app = (ofApp *)ofxGetAppPtr();

	
	/*maxZ = 0;app->cam.moveTo(dest, 5000);
	app->cam.lookAtTo(dest, 5000);*/
}

//--------------------------------------------------------------
void ofxRibbon::draw(){

	float now = ofGetElapsedTimef();
	float tweenFactor = 1;
	if (endLine)
	{
		tweenFactor = ofxeasing::map(now, tweenInFrom, tweenInTo, 1, 0, &ofxeasing::quad::easeInOut);
	}

	int w = ofGetWidth();
	int h = ofGetHeight();

    ofEnableAlphaBlending();
	ofBackground(255,255,255,255*alpha);

	ofSetColor(0);
	//ofEnableLighting();
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
		ofVec3f unitDirection = direction.normalize();

		//find both directions to the left and to the right
		ofVec3f toTheLeft = unitDirection.getRotated(-90, ofVec3f(0,0,1));
		ofVec3f toTheRight = unitDirection.getRotated(90, ofVec3f(0,0,1));

		//use the map function to determine the distance.
		//the longer the distance, the narrower the line.
		//this makes it look a bit like brush strokes
		float thickness = ofMap(distance, 0, 60, 20, 2, true);
		thickness *= tweenFactor*1.5;

		//calculate the points to the left and to the right
		//by extending the current point in the direction of left/right by the length
		ofVec3f leftPoint = thisPoint+toTheLeft*thickness;
		ofVec3f rightPoint = thisPoint+toTheRight*thickness;

		

		//add these points to the triangle strip
		mesh.addVertex(ofVec3f(leftPoint.x, leftPoint.y, leftPoint.z));
		mesh.addVertex(ofVec3f(rightPoint.x, rightPoint.y, rightPoint.z));


		float noiseR1 = ofNoise(i*1.123);// ofNoise(ofGetElapsedTimef()*0.5+i*0.123);
		float noiseR2 = ofNoise(i*2.234 + 1.23);// ofNoise(ofGetElapsedTimef()*0.5+i*0.123);

		ofColor color;
		// red to black
		/*float hue = 255 * (0.1 * abs(ofSignedNoise((i+1.23)*0.1))); 
		float brightness = 255 * (1 - abs(ofSignedNoise((i*0.2-2.324)*0.75)));*/

		// rainbow
		float hue = 255 * 0.5 * (1+sin(i*0.1));
		float brightness = 255;
		color.setHsb(hue, 255, brightness);
		mesh.addColor(color);
		//hue = 255; //255 * (0.1 * abs(ofSignedNoise((i-0.257)*0.1)));
		//brightness = 255 * (1 - abs(ofSignedNoise((i-0.324)*0.25)));
		color.setHsb(hue, 255, brightness);
		mesh.addColor(color);

	}

	//end the shape
	mesh.draw();
	//ofDisableLighting();
    
    if (bShowGui) {
        gui.draw();
    }
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
        case 'S' : gui.saveToFile(Globals::hostName + RIBBON_SETTINGS_FILE); break;
        case 'L' : gui.loadFromFile(Globals::hostName + RIBBON_SETTINGS_FILE); break;
        case 'g': bShowGui = !bShowGui; break;
	default:
		break;
	}
}
