#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetBackgroundColor(21);
    ofSetLineWidth(2);
/* not needed here
    _PresetScenes[0] = new SceneControl(&_MC);
    _PresetScenes[1] = new SceneControl(&_MC);
    _PresetScenes[2] = new SceneControl(&_MC);
    _PresetScenes[3] = new SceneControl(&_MC);
    _PresetScenes[4] = new SceneControl(&_MC);
    _PresetScenes[5] = new SceneControl(&_MC);
    _PresetScenes[6] = new SceneControl(&_MC);
    _PresetScenes[7] = new SceneControl(&_MC);
    */
    _AC = new ArtnetControl(&_MC);
    
    rl.setup(ofRectangle(100,100,60,60),ofVec2f(0,126),3,true);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    _MC.update();
    _AC->update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    _MC.drawGUI();
    _AC->drawGui();
    
    rl.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

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

void ofApp::exit()
{
    
    delete _AC;
}
