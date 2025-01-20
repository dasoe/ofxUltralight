#include "ofApp.h"

//
// This example provides communication betwen Javascript in the Browser and our OF App in both directions.
// This can be used when a heavily layouted GUI is needed for example
// 

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);

	// setup Ultralight. Params: int width, int height, [ofVec2f offset], string url/file
	// url or file is distinguished automatically by checking the beginning of the string (http(s))

	// ul.setup(1400, 700, "https://de.wikipedia.org/wiki/Eisvogel");
	ul.setup( 600, 400, ofVec2f(20,40), "file:///file.html" );

	// #### OF Stuff ### 
	positionCircle = ofVec2f(750, 400);
	positionRectangle = ofVec2f( 750, 100 );
	colors.push_back( ofColor(155, 20, 115) );
	colors.push_back(ofColor(115, 20, 145));
	whichColor = 0;
	counter = 0;

}

/* ToDo:
##########################################################
	Javascript-Stuff Direction Browser->OF
	https://docs.ultralig.ht/docs/calling-a-c-function-from-js
##########################################################
*/

// Definition
void ofApp::buttonClicked ( const JSObject& thisObject, const JSArgs& args ) {
	ofLogVerbose ( "C++ button method triggered" );
	positionCircle.y -= 10;
}

// Definition
 JSValue ofApp::buttonTwo(const JSObject& obj, const JSArgs& args) {
	ofLogVerbose("## Function 'buttonTwo' fired");
	// ## OF stuff
	positionCircle.y += 10;
	counter++;

	string str = "C++ method called " + ofToString ( counter ) + " times from JS gave back this message: Have a great day!";
	return JSValue ( str.c_str() );
}

//--------------------------------------------------------------
void ofApp::update() {
	// wait till DOM is ready, then initialize once
	// this is a bit of a hack, but enough for now
	if (ul.isDomReady() && !JSinitialized) {
		ofLogVerbose("## initializing JS");

		// Acquire the JS execution context for the current page.
		RefPtr<JSContext> context = ul.view->LockJSContext ();
		SetJSContext ( context->ctx () );

		///
		/// Get the global object (this would be the "window" object in JS)
		///
		JSObject global = JSGlobalObject ();
		global[ "callOFButtonOne" ] = BindJSCallback ( &ofApp::buttonClicked );
		global[ "callOFSecondButton" ] = BindJSCallbackWithRetval ( &ofApp::buttonTwo );
		JSinitialized = true;
	}
	// update ultralight
	ul.update();


}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);
	// draw Ultralight
	ul.draw();


	// #### OF Part ###
	ofDrawBitmapStringHighlight("openFrameworks", 750, 50 );
	// draw circle on position (changed by JS calls)
	ofDrawCircle(positionCircle, 20);
	// draw OF "Button"
	ofSetColor( colors[whichColor] );
	ofDrawRectangle( positionRectangle, 100, 30 );
	ofSetColor(255);
	ofDrawBitmapString("Button", positionRectangle.x+26, positionRectangle.y+18);


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

	// #### OF Part ###
	if (x >= positionRectangle.x && x <= positionRectangle.x + 100 && y >= positionRectangle.y && y <= positionRectangle.y+30 ) {
		// switch color while button is pressed
		whichColor = 1;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	ul.mouseReleased(x, y, button);

	// #### OF Part ###
	if (x >= positionRectangle.x && x <= positionRectangle.x + 100 && y >= positionRectangle.y && y <= positionRectangle.y + 30) {
		// switch back color (button not pressed any more)
		whichColor = 0;
		// call JS function in Browser (just as you would do inside <script> </script>)
		ul.view->EvaluateScript("receiveFromOF('Howdy! ')");
	}


}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	ul.mouseScrolled(x, y, scrollX, scrollY);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

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
