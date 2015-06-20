//
//  ofxBingQuad.cpp
//  ofVasarely
//
//  Created by Nicole & Xavier on 16/02/2015.
//
//
// https://msdn.microsoft.com/en-us/library/bb259689.aspx
//

#include "ofxBingQuad.h"
#include "ofMain.h"

//--------------------------------------------------------------
void ofxBingQuad::setup(int screenWidth, int screenHeight, int sizeOfTiles){
    
    tileSize = sizeOfTiles;
    numTilesX = ceil((double)screenWidth / (double)tileSize);
    numTilesY = ceil((double)screenWidth / (double)tileSize);
    numTilesX++; 
    levelOfDetail = log(numTilesX)/log(2);
}

//--------------------------------------------------------------
void ofxBingQuad::reset(){
    occupiedQuads.clear();
    occupiedQuadsBelow.clear();
}

//--------------------------------------------------------------
string ofxBingQuad::getDeepestQuad(int pixelX, int pixelY, int requestedDepth, int maxDepth){
    
    // quad at requested zoom
    int reqDepth = requestedDepth + levelOfDetail;
    maxLevelOfDetail = maxDepth + levelOfDetail;
    unsigned int mapsize = MapSize(levelOfDetail);
    unsigned int reqMapsize = MapSize(reqDepth);
    int reqX = ofMap(pixelX, 0, mapsize, 0, reqMapsize);
    int reqY = ofMap(pixelY, 0, mapsize, 0, reqMapsize);
    ofVec2f reqTile = PixelXYToTileXY(reqX, reqY);
    string reqQuad = TileXYToQuadKey(reqTile.x, reqTile.y, reqDepth);
    
    // Check nobody on top
    bool found = false;
    bool full = false;
    string emptyQuad;
    while (reqQuad.length()>=levelOfDetail) {
        if (isQuadOccupied(reqQuad)){
            full = true;
            break;
        }
        else if (!found){
            emptyQuad = reqQuad;
            found = true;
        }
        reqQuad = reqQuad.substr(0,reqQuad.length()-1);
    }
    
    if (found){
        if (isQuadOccupiedBelow(emptyQuad)) {
            found = false;
        }
    }

    
    if (full || !found){
        if (reqDepth < maxLevelOfDetail) {
            return getDeepestQuad(pixelX, pixelY, maxDepth, maxDepth);
        } else {
        return "";
        }
    } else {
        return emptyQuad;
    }
}

//--------------------------------------------------------------
bool ofxBingQuad::tryTakeQuad(const string& quadKey){
    
    bool isBusy = isQuadOccupiedBelow(quadKey);
    if (isBusy){
        return false;
    } else {
        string inputQuadKey = quadKey;
        occupiedQuads[inputQuadKey] = true;
        
        occupiedQuadsBelow[inputQuadKey] = true;
        while (inputQuadKey.length()>=levelOfDetail) {
            occupiedQuadsBelow[inputQuadKey] = true;
            inputQuadKey = inputQuadKey.substr(0,inputQuadKey.length()-1);
        }
        return true;
    }
}

//--------------------------------------------------------------
void ofxBingQuad::freeQuad(const string& quadKey){
    
    occupiedQuads.erase(quadKey);
    string inputQuadKey = quadKey;
    occupiedQuadsBelow[inputQuadKey] = false;
    while (inputQuadKey.length()>=levelOfDetail) {
        occupiedQuadsBelow[inputQuadKey] = false;
        inputQuadKey = inputQuadKey.substr(0,inputQuadKey.length()-1);
    }
    
}

//--------------------------------------------------------------
bool ofxBingQuad::isQuadOccupied(string quadKey){
    map<string,bool>::iterator it = occupiedQuads.find(quadKey);
    if (it != occupiedQuads.end()){
        return it->second;
    }
    return false;
}

//--------------------------------------------------------------
bool ofxBingQuad::isQuadOccupiedBelow(string quadKey){
    map<string,bool>::iterator it = occupiedQuadsBelow.find(quadKey);
    if (it != occupiedQuadsBelow.end()){
        return it->second;
    }
    return false;
}

//--------------------------------------------------------------
bool ofxBingQuad::isQuadEmpty(string quadKey){
    return !isQuadOccupied(quadKey);
}



//--------------------------------------------------------------
string ofxBingQuad::TileXYToQuadKey(int tileX, int tileY, int level){
    
    stringstream quadKey;
    for (int i = level; i > 0; i--)
        {
        char digit = '0';
        int mask = 1 << (i - 1);
        if ((tileX & mask) != 0)
            {
            digit++;
            }
        if ((tileY & mask) != 0)
            {
            digit++;
            digit++;
            }
        quadKey << digit;
        }
    return quadKey.str().c_str();
}



//--------------------------------------------------------------
ofVec2f ofxBingQuad::PixelXYToTileXY(int pixelX, int pixelY){
    
    return ofVec2f(pixelX /tileSize, pixelY / tileSize);
}

//--------------------------------------------------------------
ofVec2f ofxBingQuad::TileXYToPixelXY(int tileX, int tileY){
    
    return ofVec2f(tileX *tileSize, tileY * tileSize);
}

//--------------------------------------------------------------
unsigned int ofxBingQuad::MapSize(int level){
    
    return (unsigned int) tileSize << level;
}

//--------------------------------------------------------------
ofVec2f ofxBingQuad::QuadKeyToPixelXY(string quadKey, int targetLevel){
    
    int newTargetLevel = targetLevel+levelOfDetail;
    ofVec2f tile = QuadKeyToTileXY(quadKey);
    ofVec2f px = TileXYToPixelXY(tile.x, tile.y);
    
    unsigned int targetSize = MapSize(newTargetLevel);
    unsigned int inputSize = MapSize(quadKey.length());
    int reqX = ofMap(px.x, 0, inputSize, 0, targetSize);
    int reqY = ofMap(px.y, 0, inputSize, 0, targetSize);

    return ofVec2f(reqX,reqY);
}

//--------------------------------------------------------------
ofVec3f ofxBingQuad::QuadKeyToTileXY(string quadKey){
    
    int tileX = 0, tileY = 0;
    int lod = quadKey.length();
    for (int i = lod; i > 0; i--)
        {
        int mask = 1 << (i - 1);
        switch (quadKey[lod - i])
            {
                case '0':
                break;
                
                case '1':
                tileX |= mask;
                break;
                
                case '2':
                tileY |= mask;
                break;
                
                case '3':
                tileX |= mask;
                tileY |= mask;
                break;
                
                default:
                cout <<"-- QuadKeyToTileXY: Invalid QuadKey digit sequence (" << quadKey << ") --";
            }
        }
    
    return ofVec3f(tileX, tileY, lod);
}
