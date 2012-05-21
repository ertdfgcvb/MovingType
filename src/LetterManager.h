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

#include "cinder/Filter.h"
#include "cinder/ip/Resize.h"
#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Filesystem.h" 
#include "cinder/ImageIo.h"
#include "cinder/Xml.h"
#include "cinder/Text.h"



using namespace fs; //
using namespace ci;
using namespace ci::app;
using namespace std;

class LetterManager{
public:
    LetterManager();
	LetterManager(const string &dataPath, const string &configFile, const Font &font);
	~LetterManager();
	void update();
	void draw();
    void addLetter( string set );
    
    void newLine();
    void back();
    void clear();
    void loadSet(int setNum);
    void drawLabel();
    Color bgColor;
    
    
protected:    
    string configFile;
    string dataPath;
    
    Font font;
    
    bool textureMode;
    
    void loadConfig(int setNum);
    void loadImages(int setNum);

    map <string, vector<Surface8u> > images;
    map <string, vector<gl::Texture> > textures;
    
    struct Label {
        string name;
        string year;
        string author;
        float alpha;
    } label;
    
    struct Margin {
        float left;
        float right;
        float top;
        float bottom;
    } margin;
    
    vector<Letter> letters;
    Vec2f pos, dpos, sca, dsca;
    Vec2f insertPos;
    float fontHeight, fontLeading;
    int animationSpeed;
    bool animationLoop;
    bool checkSet(int setNum);
    vector <float> margins;
    vector <path> getFileList(string folder, int filter);
};

#endif