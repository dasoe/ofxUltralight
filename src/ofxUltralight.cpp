#include "ofxUltralight.h"

void ofxUltralight::setup(int width, int height, string url) {
	setup(width, height, ofVec2f(0, 0), url);
}

void ofxUltralight::setup(int width, int height, ofVec2f t_offset, string url) {
	//ofLogNotice(ofToDataPath("resources").c_str());
	offset = t_offset;

	config.resource_path = "../../../../addons/ofxUltralight/libs/resources";
	config.use_gpu_renderer = false;
	config.device_scale = 1.0;
	config.user_agent = "Mozilla/5.0 (Linux; Android 8.1.0; SM-G965F Build/OPM2.171019.029) AppleWebKit/537.36 (KHTML, like Gecko) SamsungBrowser/7.2 Chrome/59.0.3071.125 Mobile Safari/537.36";

	auto& platform = Platform::instance();
	platform.set_font_loader(GetPlatformFontLoader());
	platform.set_config(config);
	platform.set_logger(new MyLogger());
	// Define our custom Logger class

	//gpu_driver = make_shared<GPUDriverGL>(1);
	//platform.set_gpu_driver(gpu_driver.get());

	renderer = Renderer::Create();
	view = renderer->CreateView(width, height, false, nullptr);
	view->LoadURL(url.c_str());
	view->Focus();
}


//--------------------------------------------------------------
void ofxUltralight::update() {
	renderer->Update();
}

//--------------------------------------------------------------
void ofxUltralight::draw() {
	renderer->Render();

	///
/// Check if our Surface is dirty (pixels have changed).
///

		///
		/// Psuedo-code to upload Surface's bitmap to GPU texture.
		///

	///
	/// Cast it to a BitmapSurface.
	///
	BitmapSurface* bitmap_surface = (BitmapSurface*)(view->surface());
	if ( !bitmap_surface->dirty_bounds().IsEmpty()) {
		ofLogVerbose("dirty, so draw");

		///
	/// Get the underlying bitmap.
	///
		RefPtr<Bitmap> bitmap = bitmap_surface->bitmap();

		void* pixels = bitmap->LockPixels();

		/// Get the bitmap dimensions.
		uint32_t width = bitmap->width();
		uint32_t height = bitmap->height();
		uint32_t stride = bitmap->row_bytes();

		unsigned char* pixels2 = (unsigned char*)pixels;
		// swap R and B channels (does nnot work in ladData later, don't know why.)
		oeTexture.setSwizzle(GL_TEXTURE_SWIZZLE_R, GL_BLUE);
		oeTexture.setSwizzle(GL_TEXTURE_SWIZZLE_B, GL_RED);
		// load the pixels to ofTexture
		oeTexture.loadData(pixels2, width, height, GL_RGBA);

		/// Unlock the Bitmap when we are done.
		bitmap->UnlockPixels();

		/// Clear the dirty bounds.
		bitmap_surface->ClearDirtyBounds();
	}
	oeTexture.draw(offset.x, offset.y);


}

//--------------------------------------------------------------
void ofxUltralight::keyPressed(int key) {
	KeyEvent evt;
	evt.type = KeyEvent::kType_RawKeyDown;
	// You'll need to generate a key identifier from the virtual key code
	// when synthesizing events. This function is provided in KeyEvent.h
	GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);
	view->FireKeyEvent(evt);
	KeyEvent evt2;
	evt2.type = KeyEvent::kType_Char;
	evt2.text = ofToString(key).c_str();
	evt2.unmodified_text = ofToString(key).c_str(); // If not available, set to same as evt.text

	view->FireKeyEvent(evt2);
}

//--------------------------------------------------------------
void ofxUltralight::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofxUltralight::mouseMoved(int x, int y) {
	MouseEvent evt;
	evt.type = MouseEvent::kType_MouseMoved;
	evt.x = x - offset.x;
	evt.y = y - offset.y;
	evt.button = MouseEvent::kButton_None;

	view->FireMouseEvent(evt);
}

//--------------------------------------------------------------
void ofxUltralight::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofxUltralight::mousePressed(int x, int y, int button) {
	//cout << "Klick Mouse at " << x - offset.x << " - " << y - offset.y ;
	MouseEvent evt;
	evt.type = MouseEvent::kType_MouseDown;
	evt.x = x - offset.x;
	evt.y = y - offset.y;
	evt.button = MouseEvent::kButton_Left;

	view->FireMouseEvent(evt);
}

//--------------------------------------------------------------
void ofxUltralight::mouseReleased(int x, int y, int button) {
	MouseEvent evt;
	evt.type = MouseEvent::kType_MouseUp;
	evt.x = x - offset.x;
	evt.y = y - offset.y;
	evt.button = MouseEvent::kButton_Left;

	view->FireMouseEvent(evt);
}

//--------------------------------------------------------------
void ofxUltralight::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofxUltralight::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofxUltralight::mouseScrolled(int x, int y, float scrollX, float scrollY) {

}

//--------------------------------------------------------------
void ofxUltralight::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofxUltralight::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofxUltralight::dragEvent(ofDragInfo dragInfo) {

}
