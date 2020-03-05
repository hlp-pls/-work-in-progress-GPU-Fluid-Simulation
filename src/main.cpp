#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLWindowSettings settings;
    settings.setGLVersion(2, 1); // Fixed pipeline
    
    settings.setSize(1024, 768);
    ofCreateWindow(settings);
    ofSetFullscreen(true);
    
    ofRunApp(new ofApp());
}
