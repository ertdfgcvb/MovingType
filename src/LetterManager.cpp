//
//  LetterManager.cpp
//  movingType
//
//  Created by Andreas on 27.03.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "LetterManager.h"

//move to main app
LetterManager::LetterManager(){}
LetterManager::LetterManager(const string &dataPath, const string &configFile){
    this->dataPath = dataPath;
    this->configFile = configFile;    
}

LetterManager::~LetterManager(){
    letters.clear();
    images.clear();
    textures.clear();
}

void LetterManager::clear(){
    letters.clear();
    currentPos.set(margin.left, margin.top);
}

void LetterManager::update(){
	for(vector<Letter>::iterator it = letters.begin(); it != letters.end();){
		Letter& letter = *it;
		if(letter.isDead()){
			letters.erase( it );
		}else{
			letter.update();
			++it;
		}
	}
}

void LetterManager::draw(){
	for(vector<Letter>::iterator it = letters.begin(); it != letters.end(); ++it){
		it->draw();
	}
}

void LetterManager::addLetter( const vector<Surface8u> &sequence ){
    Letter l(currentPos, sequence );
    l.configure(fontHeight, animationSpeed, animationLoop);
	letters.push_back( l );
    currentPos.x += l.getWidth();
    if (currentPos.x >= getWindowWidth() - margin.right) newLine();
}

void LetterManager::addLetter( const vector<gl::Texture> &sequence ){
    Letter l(currentPos, sequence );
    l.configure(fontHeight, animationSpeed, animationLoop);
	letters.push_back( l );
    currentPos.x += l.getWidth();
    if (currentPos.x >= getWindowWidth() - margin.right) newLine();
}

void LetterManager::addLetter( string letterName ){
    if (!textureMode && images[letterName].size() > 0) {
        addLetter(images[letterName]);
    } else if(textureMode && textures[letterName].size() > 0) {
        addLetter(textures[letterName]);
    }        
}

void LetterManager::newLine(){
    currentPos.x = margin.left;
    currentPos.y += fontHeight + fontLeading;
}

void LetterManager::back(){
    if (letters.size() > 0) {
        Letter l = letters.back();
        currentPos.set(l.getPos());
        letters.pop_back();
    }
}

vector <path> LetterManager::getFileList(string folder, int filter=0){ // 0=all, 1=folders, 2=files
    vector<path> res;    
    path p( folder ); 
    if (is_directory(p)){
        for( directory_iterator it( p ); it != directory_iterator(); ++it ) {
            if ( (filter==0) || (filter==1 && is_directory( *it )) || (filter==2 && is_regular_file(*it))){
                res.push_back(it->path());   
            }
        }
        sort(res.begin(), res.end());
    }
    return res;
}

void LetterManager::loadSet(int setNum){
    loadConfig(setNum);
    loadImages(setNum);    
    clear();
}

void LetterManager::loadConfig(int setNum){
    path p( dataPath );    
    
    if (exists(p) && is_directory(p)){
        console() << "---------------------------------------------------------------\n";
        typedef vector<path> vec; 
        vec sub = getFileList(p.string(), 1); //folders
        console() << "Found: " << sub.size() <<" sets.\n";
        if (setNum < sub.size()){
            path folder = sub.at(setNum);
            XmlTree doc;
            //config file
            console() << "Loading config file [" << folder.string() + "/" + configFile << "]...";
            try{
                XmlTree doc( loadFile( folder.string() + "/" + configFile) );
                console() << "OK\n";
                //rewrite...            
                try{
                    XmlTree cfg = doc.getChild( "config" );
                    fontHeight     = cfg.getChild("font").getAttributeValue<float>( "height", 80.0f );
                    fontLeading    = cfg.getChild("font").getAttributeValue<float>( "leading", 10.0f );
                    animationSpeed = cfg.getChild("animation").getAttributeValue<int>( "speed", 80.0f );
                    animationLoop  = cfg.getChild("animation").getAttributeValue<bool>( "loop", 10.0f );
                    margin.left    = cfg.getChild("margin").getAttributeValue<float>( "left", 10.0f );
                    margin.right   = cfg.getChild("margin").getAttributeValue<float>( "right", 10.0f );
                    margin.top     = cfg.getChild("margin").getAttributeValue<float>( "top", 10.0f );
                    margin.bottom  = cfg.getChild("margin").getAttributeValue<float>( "bottom", 10.0f );        
                    textureMode    = cfg.getChild("system").getAttributeValue<bool>( "textureMode", false );  
                    float r        = cfg.getChild("background").getAttributeValue<float>( "r", 0.0f ); 
                    float g        = cfg.getChild("background").getAttributeValue<float>( "g", 0.0f );
                    float b        = cfg.getChild("background").getAttributeValue<float>( "b", 0.0f );                
                    bgColor = Color(r,g,b);
                    
                    console() << "OK\n";
                } catch(...){
                    console() << "ERROR\n";
                }        
            } catch(...){
                console() << "ERROR\n ";    
            }
            console() << "Parsing config file... ";                
        } else {
            console() << "Set [" << setNum << "] doesn't exist!\n";
        }
    } else {
        console() << "Data folder [" << dataPath << "] not found!\n";
    }
}


void LetterManager::loadImages(int setNum){
    letters.clear();
    images.clear();
    textures.clear();
    
    path p( dataPath );    
    
    if (exists(p) && is_directory(p)){
        console() << "---------------------------------------------------------------\n";
        typedef vector<path> vec; 
        vec sub = getFileList(p.string(), 1); //folders
        console() << "Found: " << sub.size() <<" sets.\n";
        if (setNum < sub.size()){
            path folder = sub.at(setNum);
            console() << "Loading set [" << folder.string() << "]...\n";
            
            vec sets = getFileList(folder.string(), 1); //folders
            
            for (vec::const_iterator it1 (sets.begin()); it1 != sets.end(); ++it1){
                string setPath = it1->string();
                string setName = it1->filename().string();
                console() << "Scanning set [" << setPath << "]...\n";
                vec files = getFileList(setPath, 2); //files
                for (vec::const_iterator it2 (files.begin()); it2 != files.end(); ++it2){
                    string file = it2->string(); 
                    stringstream s;
                    s << "Loading file: " << file << " [" << fs::file_size( *it2 ) << "kb]... ";  
                    console() << s.str();                     
                    try{
                        Surface8u surf = loadImage(DataSourcePath::create( file ));
                        images[setName].push_back(surf);                            
                        if (textureMode){
                            textures[setName].push_back(gl::Texture(surf));    
                        }
                        console() << " OK\n";   
                    } catch( ... ) {                        
                        console() << " SKIPPED\n";   
                    }
                }                            
            }
        } else {
            console() << "Set [" << setNum << "] doesn't exist!";
        }
    } else {
        console() << "Data folder [" << dataPath << "] not found!";
    }
}