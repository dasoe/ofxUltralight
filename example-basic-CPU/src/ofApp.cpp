#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// setup params: int width, int height, [ofVec2f offset], string url/file
	// url / file ist automatically distinguished checking beginning of string (http(s))

	ul.setup(1400, 700, "https://de.wikipedia.org/wiki/Eisvogel");
	// ul.setup( 600, 400, "file.html" );

}

//--------------------------------------------------------------
void ofApp::update() {
	ul.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ul.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ul.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	ul.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	ul.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	ul.mouseReleased(x, y, button);
}


//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
