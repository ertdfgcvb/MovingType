//
//  Letter.cpp
//  movingType
//
//  Created by Andreas on 27.03.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Letter.h"
Letter::Letter(){};
Letter::Letter(const Vec2f &pos, const vector<Surface8u> &images){
	this->pos = Vec2f(pos);
	this->images = images;
    textureMode = false;
    numFrames = images.size();
    //std::cout << "addr: " << &images[0] << " textureMode=false\n";
}

Letter::Letter(const Vec2f &pos, const vector<gl::Texture> &textures){
	this->pos = Vec2f(pos);
	this->textures = textures;
    textureMode = true;
    numFrames = textures.size();
    //std::cout << "addr: " << &textures[0] << " textureMode=true\n";
}

void Letter::configure(float height, int speed, int loop){
    setHeight(height);
    frameCount = 0;
    this->speed = max(1,speed);
    this->loop = loop;
    dir = 1;
}

void Letter::update(){
    if (loop == 1) {
        currentFrame = (frameCount / speed) % numFrames;
    } else if (loop == 2) {
        
        currentFrame = frameCount / speed;
        if (currentFrame >= numFrames) {
            currentFrame = numFrames - 1;
            dir = -1;
        } else if (currentFrame < 0) {
            currentFrame = 0;
            dir = 1;
        }
    } else {
        currentFrame = min(frameCount / speed, numFrames-1);
    }
    frameCount+=dir;    
}

void Letter::setHeight(float h){
    float a;
    if (!textureMode) a = images[0].getAspectRatio();
    else a = textures[0].getAspectRatio();    
    float w = h * a;
    dim.set(w, h);
}

bool Letter::isInView(const Vec2f &offset){
    Vec2f screenPos = Vec2f(offset);
    screenPos += pos;
    
    if (screenPos.x + dim.x < 0 || screenPos.x > getWindowWidth() || screenPos.y + dim.y < 0 || screenPos.y > getWindowHeight()){ 
        return false;
    }
    return true;
}

void Letter::setPos(const Vec2f &pos){
    this->pos = Vec2f(pos);
}

Vec2f Letter::getPos(){
    return pos;
}

Vec2f Letter::getSize(){
    return dim;
} 

void Letter::draw(){
    if (!textureMode) gl::draw( gl::Texture(images[currentFrame]), Area(pos.x, pos.y, pos.x + dim.x, pos.y + dim.y) );
    else  gl::draw( textures[currentFrame], Area(pos.x, pos.y, pos.x + dim.x, pos.y + dim.y) );
}

bool Letter::isDead(){
    return false;
}
