//
//  Letter.h
//  movingType
//
//  Created by Andreas on 27.03.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef movingType_Letter_h
#define movingType_Letter_h

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"
#include <vector>

using namespace ci;
using namespace std;

class Letter{

public:
	Letter(const Vec2f &pos, const vector<Surface8u> &images);
	Letter(const Vec2f &pos, const vector<gl::Texture> &textures);

    void configure(float height, int speed, bool loop);

    
	void update();
	void draw();
    void setHeight(float h);
    float getWidth();
	bool isDead();
    Vec2f getPos();
    
protected:
    int frameCount, speed, currentFrame, numFrames;
    bool loop;
    Vec2f dim;
	Vec2f pos;
	vector<Surface8u> images;
	vector<gl::Texture> textures;
    bool textureMode;
};

#endif
