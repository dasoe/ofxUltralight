#include "ofApp.h"

//
// This example show basic integrating of a Browser in an OF App.
// 

//--------------------------------------------------------------
void ofApp::setup() {

	// setup Ultralight. Params: int width, int height, [ofVec2f offset], string url/file
	// url or file is distinguished automatically by checking the beginning of the string (http(s))

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
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	ul.mouseScrolled(x, y, scrollX, scrollY);
}