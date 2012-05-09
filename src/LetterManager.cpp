//
//  LetterManager.cpp
//  movingType
//
//  Created by Andreas on 27.03.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "LetterManager.h"

LetterManager::LetterManager(){
}

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
    insertPos.set(margin.left, margin.top);
    pos.set(0,0);
    dpos.set(0,0);
    sca.set(1,1);
    dsca.set(1,1);
}

void LetterManager::update(){
	for(vector<Letter>::iterator it = letters.begin(); it != letters.end();){
		Letter& letter = *it;
		if(letter.isInView(pos)){
			letter.update();
			++it;
		}else{
			letters.erase( it );
		}
	}
    pos += (dpos - pos) * 0.05f; //LOVE!
    sca += (dsca - sca) * 0.05f;     
}

void LetterManager::draw(){
        
    gl::pushModelView ();
    gl::translate(pos);
    gl::scale(sca);
    
	for(vector<Letter>::iterator it = letters.begin(); it != letters.end(); ++it){
		it->draw();
	}
    
    gl::popModelView();
}

void LetterManager::addLetter( string letterName ){
    //Caution: total mess here... need to figure out how things work
    //Q1: possibility of a constructor with generic type?
    //Q2: how to create an empty class var and test it (null?)?
    Letter l;
    bool init = false;
    if (!textureMode && images[letterName].size() > 0) {
        l = Letter(insertPos, images[letterName]);
        init = true;
    } else if(textureMode && textures[letterName].size() > 0) {
        l = Letter(insertPos, textures[letterName]);
        init = true;
    }
    if (init) {        
        l.configure(fontHeight, animationSpeed, animationLoop);	
        if (l.getPos().x + l.getSize().x >= getWindowWidth() - margin.right){
            newLine();
            l.setPos(insertPos);
        }
        
        letters.push_back(l); //why does this not work as expected?
        
        insertPos.x += l.getSize().x;
        if (insertPos.x >= getWindowWidth() - margin.right) newLine();
        
        if (insertPos.y + fontHeight + dpos.y >= getWindowHeight() - margin.bottom){
            dpos.y -= getWindowHeight() / 2;
        }
    }
}

void LetterManager::newLine(){
    insertPos.x = margin.left;
    insertPos.y += fontHeight + fontLeading;
}

void LetterManager::back(){
    if (letters.size() > 0) {
        Letter l = letters.back();
        insertPos.set(l.getPos());
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
//                        int h = (int) fontHeight;
//                        int w = (int) fontHeight * surf.getAspectRatio();
//                        Surface8u scaled( w, h, false );
//                        ip::resize(surf, scaled, FilterTriangle(1.0f));
                        images[setName].push_back(surf);                            
                        if (textureMode){
                            gl::Texture t = gl::Texture(surf);
                            textures[setName].push_back(t);    
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