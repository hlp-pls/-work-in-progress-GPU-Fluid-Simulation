#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    time_step = 0.5;
    diffusion = 0.0;
    viscosity = 0.0000001;
    radius = 2.0;
    amount = 1.0;
    
    width = ofGetWidth();
    height = ofGetHeight();
    
    cout << width << ", " << height;
    
    //create ofShader for rendering the screen
    sh_render.load("shaders/render.vert","shaders/render.frag");
    
    sh_up_v0_diff.load("","shaders/up_v0_diff.frag");
    sh_up_v_advec.load("","shaders/up_v_advec.frag");
    
    sh_up_s_diff.load("","shaders/up_s_diff.frag");
    sh_up_color_advec.load("","shaders/up_color_advec.frag");
    
    sh_addDensity.load("","shaders/addDensity.frag");
    sh_addColor.load("","shaders/addColor.frag");
    sh_addVelocity.load("","shaders/addVelocity.frag");
    
    //create initial data for each textures
    vector<float> init(width*height*3);
    
    //create pingpongbuffers for each textures
        //color
    color_PingPong.allocate(width, height, GL_RGB32F);
    color_PingPong.src->getTexture().loadData(init.data(), width, height, GL_RGB32F);
    color_PingPong.dst->getTexture().loadData(init.data(), width, height, GL_RGB32F);
        //v
    v_PingPong.allocate(width, height, GL_RGB32F);
    v_PingPong.src->getTexture().loadData(init.data(), width, height, GL_RGB32F);
    v_PingPong.dst->getTexture().loadData(init.data(), width, height, GL_RGB32F);
        //v0
    v0_PingPong.allocate(width, height, GL_RGB32F);
    v0_PingPong.src->getTexture().loadData(init.data(), width, height, GL_RGB32F);
    v0_PingPong.dst->getTexture().loadData(init.data(), width, height, GL_RGB32F);
        //s,density
    sd_PingPong.allocate(width, height, GL_RGB32F);
    sd_PingPong.src->getTexture().loadData(init.data(), width, height, GL_RGB32F);
    sd_PingPong.dst->getTexture().loadData(init.data(), width, height, GL_RGB32F);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //texture number and values
        //--> 1 --> v
        //--> 2 --> v0
        //--> 3 --> color
        //--> 4 --> sd
    
    //diffuse v0
    v0_PingPong.dst->begin();
    ofClear(0);
    sh_up_v0_diff.begin();
    sh_up_v0_diff.setUniformTexture("v_texture",v_PingPong.src->getTexture(),1);
    sh_up_v0_diff.setUniformTexture("v0_texture",v0_PingPong.src->getTexture(),2);
    sh_up_v0_diff.setUniform2f("resolution", (float)width, (float)height);
    sh_up_v0_diff.setUniform1f("time_step",time_step);
    sh_up_v0_diff.setUniform1f("viscosity",viscosity);
    
    v0_PingPong.src->draw(0, 0);
    
    sh_up_v0_diff.end();
    v0_PingPong.dst->end();
    v0_PingPong.swap();
    
    
    //advect v
    v_PingPong.dst->begin();
    ofClear(0);
    sh_up_v_advec.begin();
    sh_up_v_advec.setUniformTexture("v_texture",v_PingPong.src->getTexture(),1);
    sh_up_v_advec.setUniformTexture("v0_texture",v0_PingPong.src->getTexture(),2);
    sh_up_v_advec.setUniform2f("resolution", (float)width, (float)height);
    sh_up_v_advec.setUniform1f("time_step",time_step);
    
    v_PingPong.src->draw(0, 0);
    
    sh_up_v_advec.end();
    v_PingPong.dst->end();
    v_PingPong.swap();
    
    //diffuse s
    sd_PingPong.dst->begin();
    ofClear(0);
    sh_up_s_diff.begin();
    sh_up_s_diff.setUniformTexture("color_texture",color_PingPong.src->getTexture(),3);
    sh_up_s_diff.setUniformTexture("sd_texture",sd_PingPong.src->getTexture(),4);
    sh_up_s_diff.setUniform2f("resolution", (float)width, (float)height);
    sh_up_s_diff.setUniform1f("time_step",time_step);
    sh_up_s_diff.setUniform1f("diffusion",diffusion);
    
    sd_PingPong.src->draw(0, 0);
    
    sh_up_s_diff.end();
    sd_PingPong.dst->end();
    sd_PingPong.swap();
    
    //advect color
    color_PingPong.dst->begin();
    ofClear(0);
    sh_up_color_advec.begin();
    sh_up_color_advec.setUniformTexture("v_texture",v_PingPong.src->getTexture(),1);
    sh_up_color_advec.setUniformTexture("color_texture",color_PingPong.src->getTexture(),3);
    sh_up_color_advec.setUniformTexture("sd_texture",sd_PingPong.src->getTexture(),4);
    sh_up_color_advec.setUniform2f("resolution", (float)width, (float)height);
    sh_up_color_advec.setUniform1f("time_step",time_step);
    
    color_PingPong.src->draw(0, 0);
    
    sh_up_color_advec.end();
    color_PingPong.dst->end();
    color_PingPong.swap();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);

    sh_render.begin();
    sh_render.setUniformTexture("v_texture", v_PingPong.src->getTexture(), 1);
    sh_render.setUniformTexture("v0_texture", v0_PingPong.src->getTexture(), 2);
    sh_render.setUniformTexture("color_texture", color_PingPong.src->getTexture(), 3);
    sh_render.setUniformTexture("sd_texture", sd_PingPong.src->getTexture(), 4);
    sh_render.setUniform2f("resolution", (float)width, (float)height);
    //mesh.draw();
    ofDrawRectangle(0, 0, width, height);
    
    sh_render.end();
    
    color_PingPong.src->getTexture().draw(0,0,width*0.2,height*0.2);
    sd_PingPong.src->getTexture().draw(width*0.2,0,width*0.2,height*0.2);
    v_PingPong.src->getTexture().draw(width*0.2*2,0,width*0.2,height*0.2);
    v0_PingPong.src->getTexture().draw(width*0.2*3,0,width*0.2,height*0.2);
    
    ofSetColor(255);
    ofDrawLine(0,height*0.2,width*0.2*4,height*0.2);
    ofDrawLine(width*0.2,0,width*0.2,height*0.2);
    ofDrawLine(width*0.2*2,0,width*0.2*2,height*0.2);
    ofDrawLine(width*0.2*3,0,width*0.2*3,height*0.2);
    ofDrawLine(width*0.2*4,0,width*0.2*4,height*0.2);
    
    ofSetColor(255,0,0);
    ofDrawBitmapString("Fps: " + ofToString( ofGetFrameRate()), 15, height-15);
    ofDrawBitmapString("color", 15, 15);
    ofDrawBitmapString("density", width*0.2+15, 0+15);
    ofDrawBitmapString("velocity", width*0.2*2+15,0+15);
    ofDrawBitmapString("velocity0", width*0.2*3+15,0+15);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    float mX = ofGetMouseX();
    float mY = ofGetMouseY();
    float amtX = ofGetMouseX()-ofGetPreviousMouseX();
    float amtY = ofGetMouseY()-ofGetPreviousMouseY();
    
    //add density
    sd_PingPong.dst->begin();
    ofClear(0);
    sh_addDensity.begin();
    sh_addDensity.setUniformTexture("sd_texture",sd_PingPong.src->getTexture(),4);
    sh_addDensity.setUniform2f("mpos", mX, mY);
    sh_addDensity.setUniform1f("amount", amount);
    sh_addDensity.setUniform2f("resolution",(float)width, (float)height);
    sh_addDensity.setUniform1f("radius",radius);
    
    sd_PingPong.src->draw(0, 0);
    
    sh_addDensity.end();
    sd_PingPong.dst->end();
    sd_PingPong.swap();
    
    //add color
    color_PingPong.dst->begin();
    ofClear(0);
    sh_addColor.begin();
    sh_addColor.setUniformTexture("color_texture",color_PingPong.src->getTexture(),3);
    sh_addColor.setUniformTexture("sd_texture",sd_PingPong.src->getTexture(),4);
    sh_addColor.setUniform2f("mpos",mX,mY);
    sh_addColor.setUniform2f("resolution",(float)width, (float)height);
    sh_addColor.setUniform1f("radius",radius);
    
    color_PingPong.src->draw(0, 0);
    
    sh_addColor.end();
    color_PingPong.dst->end();
    color_PingPong.swap();
    
    //add velocity
    v_PingPong.dst->begin();
    ofClear(0);
    sh_addVelocity.begin();
    sh_addVelocity.setUniformTexture("v_texture",v_PingPong.src->getTexture(),1);
    sh_addVelocity.setUniform2f("mpos",mX,mY);
    sh_addVelocity.setUniform2f("amt",amtX,amtY);
    sh_addVelocity.setUniform2f("resolution",(float)width, (float)height);
    sh_addVelocity.setUniform1f("radius",radius);
    
    v_PingPong.src->draw(0, 0);
    
    sh_addVelocity.end();
    v_PingPong.dst->end();
    v_PingPong.swap();
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
