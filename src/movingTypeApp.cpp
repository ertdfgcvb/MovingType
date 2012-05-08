#include <vector>

#include "cinder/app/AppBasic.h"
#include "cinder/Filesystem.h" 
#include "cinder/Text.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"

#include "LetterManager.h"

using namespace fs; //
using namespace ci;
using namespace ci::app;
using namespace std;

const string DATA_FOLDER = "data";
const string CONFIG_FILE = "config.xml";
const string FONT = "MISO-REG.OTF";

class movingTypeApp : public AppBasic {
private:
    Font font;
    LetterManager lm;
    
	void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();
	void mouseDown( MouseEvent event );	
    void keyUp( KeyEvent event );
    void keyDown( KeyEvent event );
    
    void drawLabel( string *s );
    void loadSet( int setNum );  
};
/*
 - (std::string)getAppPath{
 NSString *resultPath = [[NSBundle mainBundle] bundlePath];
 std::string result;
 result = [resultPath cStringUsingEncoding:NSUTF8StringEncoding];
 return result;
 }
 */

void movingTypeApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 900, 700 );
    settings->setFrameRate( 60.0f );
    //settings->setFullScreen( true );
    //settings->setResizable( false );  
}

void movingTypeApp::setup(){
    path p = getAppPath();
    string absolutePath = p.parent_path().string() + "/" + DATA_FOLDER;
    font = Font(loadResource( FONT ), 24 );
    lm = LetterManager(absolutePath, CONFIG_FILE);
    lm.loadSet(0);
}

void movingTypeApp::drawLabel(string *s){    
    TextLayout simple;
    simple.setFont( font );
    simple.setColor( Color( 1,1,1 ) );
    simple.addLine( *s );
    gl::draw( simple.render( true, true ) , Vec2f( 20, 20 ) ); 
}


void movingTypeApp::mouseDown( MouseEvent event ){
}

void movingTypeApp::update(){
    lm.update();
}

void movingTypeApp::draw(){
    gl::clear( lm.bgColor ); 
    lm.draw();
}

void movingTypeApp::keyDown(KeyEvent event ) {
    
    if (event.isMetaDown()){
        if( event.getChar() == 'f') setFullScreen(!isFullScreen());
        else if( event.getChar() == 'x') lm.clear();
    } else {
        
        char c = event.getChar();
        int n = (int) c;
        
        if (n >= 0) { //needs a fix for ç and è etc.
            stringstream s;
            
            if (n == 13){
                lm.newLine();           
            } else if (n == 127){
                lm.back();           
            } else if (n >=97 && n <= 122) {
                s << "lowercase_" << c;
            } else if (n >= 65 && n <= 90) {
                s << "uppercase_" << (char)(c+32);
            } else if (n >= 48 && n <= 57) {
                s << "number_" << c;
            } else if (n == 32) {
                s << "space";
            } else {
                s << "char_" << n;
            }
            console() << "loading [" << n <<"]: " << s.str() <<"\n";
            lm.addLetter(s.str());
        }
    }
}

void movingTypeApp::keyUp( KeyEvent event ) {
    if( event.getCode() == KeyEvent::KEY_F1 ) lm.loadSet(0);
    else if( event.getCode() == KeyEvent::KEY_F2 ) lm.loadSet(1);
    else if( event.getCode() == KeyEvent::KEY_F3 ) lm.loadSet(2);
    else if( event.getCode() == KeyEvent::KEY_F4 ) lm.loadSet(3);
    else if( event.getCode() == KeyEvent::KEY_F5 ) lm.loadSet(4);
    else if( event.getCode() == KeyEvent::KEY_F6 ) lm.loadSet(5);
    else if( event.getCode() == KeyEvent::KEY_F7 ) lm.loadSet(6);
    else if( event.getCode() == KeyEvent::KEY_F8 ) lm.loadSet(7);
    else if( event.getCode() == KeyEvent::KEY_F9 ) lm.loadSet(8);
    else if( event.getCode() == KeyEvent::KEY_F10 ) lm.loadSet(9);
    else if( event.getCode() == KeyEvent::KEY_F11 ) lm.loadSet(10);
    else if( event.getCode() == KeyEvent::KEY_F12 ) lm.loadSet(11);
}


CINDER_APP_BASIC( movingTypeApp, RendererGl )