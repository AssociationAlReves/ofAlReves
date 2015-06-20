//
//  ofxBingQuad.h
//  ofVasarely
//
//  Created by Nicole & Xavier on 16/02/2015.
//
//
#pragma once

#include "ofMain.h"

class ofxBingQuad {
    
public:
    
    void setup(int width, int height, int sizeOfTiles = 256);
    

    
    string TileXYToQuadKey(int tileX, int tileY, int level);
    ofVec2f PixelXYToTileXY(int pixelX, int pixelY);
    ofVec2f TileXYToPixelXY(int tileX, int tileY);
    ofVec3f QuadKeyToTileXY(string quadKey);
    unsigned int MapSize(int level);
    
    int levelOfDetail;
    int maxLevelOfDetail;
    
    string getDeepestQuad(int pixelX, int pixelY, int requestedDepth, int maxDepth); // returns empty string if not found
    bool tryTakeQuad(const string& quadKey);
    ofVec2f QuadKeyToPixelXY(string quadKey, int targetLevel);
    void freeQuad(const string& quadKey);
    
    void reset();

    
private:
    map<string,bool> occupiedQuads;
    map<string,bool> occupiedQuadsBelow;
    
    bool isQuadOccupied(string quadKey);
    bool isQuadOccupiedBelow(string quadKey);
    bool isQuadEmpty(string quadKey);
    
    int numTilesX;
    int numTilesY;
    int tileSize;
    
    
    
};