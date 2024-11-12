#pragma once

#include "ofMain.h"
#include "ofxUltralight.h"


class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		bool JSinitialized;
		JSValueRef  buttonOne(JSContextRef ctx, JSObjectRef function,
			JSObjectRef thisObject, size_t argumentCount,
			const JSValueRef arguments[], JSValueRef* exception);
		void buttonTwo(const JSObject& obj, const JSArgs& args);
		ofxUltralight ul;

		ofVec2f positionCircle, positionRectangle;
		vector <ofColor> colors;
		unsigned int whichColor;
};
