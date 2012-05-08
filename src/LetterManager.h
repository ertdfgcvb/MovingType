//
//  LetterManager.h
//  movingType
//
//  Created by Andreas on 27.03.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef movingType_LetterManager_h
#define movingType_LetterManager_h

#include <map>
#include <vector>

#include "Letter.h"

#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Filesystem.h" 
#include "cinder/ImageIo.h"
#include "cinder/Xml.h"


using namespace fs; //
using namespace ci;
using namespace ci::app;
using namespace std;

class LetterManager{
public:
    LetterManager();
	LetterManager(const string &dataPath, const string &configFile);
	~LetterManager();
	void update();
	void draw();
    void addLetter( string set );
    
    void newLine();
    void back();
    void clear();
    void loadSet(int setNum);
    Color bgColor;
    
    
protected:    
    string configFile;
    string dataPath;
    
    bool textureMode;
    
    void loadConfig(int setNum);
    void loadImages(int setNum);

    void addLetter( const vector<Surface8u> &sequence );
    map <string, vector<Surface8u> > images;
    
    void addLetter( const vector<gl::Texture> &sequence );
    map <string, vector<gl::Texture> > textures;
    
    struct Margin {
        float left;
        float right;
        float top;
        float bottom;
    } margin;
    
    vector<Letter> letters;
    Vec2f currentPos;
    float fontHeight, fontLeading;
    int animationSpeed;
    bool animationLoop;
    vector <float> margins;
    vector <path> getFileList(string folder, int filter);
};

#endif