//
//  Letter.cpp
//  movingType
//
//  Created by Andreas on 27.03.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Letter.h"

Letter::Letter(const Vec2f &pos, const vector<Surface8u> &images){
    textureMode = false;
    numFrames = images.size();
	this->pos = Vec2f(pos);
	this->images = images;
    //std::cout << "addr: " << &images[0] << " textureMode=false\n";
}

Letter::Letter(const Vec2f &pos, const vector<gl::Texture> &textures){
    textureMode = true;
    numFrames = textures.size();
	this->pos = Vec2f(pos);
	this->textures = textures;
    //std::cout << "addr: " << &textures[0] << " textureMode=true\n";
}

void Letter::configure(float height, int speed, bool loop){
    setHeight(height);
    frameCount = 0;
    if (speed < 1) speed = 1;
    this->speed = speed;
    this->loop = loop;    
}

void Letter::update(){
    frameCount++;
    currentFrame = frameCount / speed;
    if (loop) {
        currentFrame = currentFrame % numFrames;
    } else {
        currentFrame = min(currentFrame, numFrames-1);
    }
}

void Letter::setHeight(float h){
    float a;
    if (!textureMode) a = images[0].getAspectRatio();
    else a = textures[0].getAspectRatio();
    
    float w = h * a;
    dim.set(w, h);
}

Vec2f Letter::getPos(){
    return pos;
}

float Letter::getWidth(){
    return dim.x;
}

void Letter::draw(){
    if (!textureMode) gl::draw( gl::Texture(images[currentFrame]), Area(pos.x, pos.y, pos.x + dim.x, pos.y + dim.y) );
    else  gl::draw( textures[currentFrame], Area(pos.x, pos.y, pos.x + dim.x, pos.y + dim.y) );
}

bool Letter::isDead(){
	return false;
}
