#include "ofxUltralight.h"


void ofxUltralight::setup(int width, int height, string url) {
	setup(width, height, ofVec2f(0, 0), url);
	DOMready = false;
}

void ofxUltralight::setup(int width, int height, ofVec2f t_offset, string url) {
	//ofLogNotice(ofToDataPath("resources").c_str());
	offset = t_offset;
	Config config;

	//config.resource_path_prefix = "../../../../addons/ofxUltralight/libs/resources/";

	auto& platform = Platform::instance();
	platform.set_config(config);
	platform.set_font_loader(GetPlatformFontLoader());
	platform.set_logger(GetDefaultLogger("ultralight.log"));
	platform.set_file_system(GetPlatformFileSystem("data"));

	///
	/// You should keep this instance alive for the duration of your program.
	///
	std::unique_ptr<GLTextureSurfaceFactory> factory ( new GLTextureSurfaceFactory () );

	Platform::instance ().set_surface_factory ( factory.get () );

	//gpu_driver = make_shared<GPUDriverGL>(1);
	//platform.set_gpu_driver(gpu_driver.get());

	renderer = Renderer::Create();

	ViewConfig view_config;
	view_config.is_accelerated = false;
	view_config.initial_device_scale = 1.0;


	view = renderer->CreateView(width, height, view_config, nullptr);
	ofLogVerbose("given string is '" + url + "'");
	
	// #### New: Custom Surface: paint directly into ofTexture
	// Preparation of Texture. Possible ToDo: Does this need to be adjusted later in some case?
	oeTexture.texData.width = view->width ();
	oeTexture.texData.height = view->height ();
	oeTexture.texData.tex_w = oeTexture.texData.width;
	oeTexture.texData.tex_h = oeTexture.texData.height;
	oeTexture.texData.bFlipTexture = false;
	oeTexture.texData.tex_u = 1;
	oeTexture.texData.tex_t = 1;
	oeTexture.texData.textureTarget = GL_TEXTURE_2D;
	oeTexture.texData.bAllocated = true;

	//inspectorView = view->inspector();
	//inspectorView->Resize(700, 300);
	//string ipath = ofToDataPath("", true) + "/inspector/Main.html";
	//inspectorView->LoadURL( ipath.c_str() );

	load(url);


}

//--------------------------------------------------------------
void ofxUltralight::load(string url) {
	DOMready = false;

	bool isURL = false;
	// there seem to be problem with colons, so some workaround. Feel free to fix it...
	if (url.rfind("http", 0) == 0 && url.rfind("//", 6)) {
		ofLogVerbose("starts with http:// ... set it as a URI");
		isURL = true;
	}
	if (url.rfind("https", 0) == 0 && url.rfind("//", 7)) {
		ofLogVerbose("starts with https:// ... set it as a URI");
		isURL = true;
	}
	if (url.rfind("file", 0) == 0 && url.rfind("///", 6)) {
		ofLogVerbose("starts with file:/// ... set it as a URI");
		isURL = true;
	}

	if (isURL) {
		view->LoadURL (url.c_str());
	}
	else {
		ofLogVerbose("maybe it's still a File - try to load it");
		ofBuffer buffer = ofBufferFromFile(url);
		string content;
		if (buffer.size()) {
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {

				content += *it;

			}
			view->LoadHTML(content.c_str());
		}
		else {
			view->LoadHTML( url.c_str() );
		}
	}
	view->Focus();
	view->set_load_listener(this);
	view->set_view_listener(this);
}

//--------------------------------------------------------------
void ofxUltralight::OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const String& url) {
	ofLogVerbose( "Ultralight DOM ready" );
	DOMready = true;

	///
	/// Acquire the JS execution context for the current page.
	///
	auto scoped_context = caller->LockJSContext();

	///
	/// Typecast to the underlying JSContextRef.
	///
	jsContext = (*scoped_context);
}

bool ofxUltralight::isDomReady() {
	return DOMready;
}

JSContextRef ofxUltralight::getJSContext() {
	if (DOMready) {
		return jsContext;
	}
	else {
		return NULL;
	}
}

//--------------------------------------------------------------
void ofxUltralight::OnAddConsoleMessage(
	View* caller,
	const ConsoleMessage& message 
) {
	
	// give out the console Message via OF Logging
	auto getMessage = (string) Stringify( message.source() );
	ofLogNotice( "[Ultralight: Console]: [" + getMessage + "] [" + Stringify( message.level() ) + "] " + ToUTF8( message.message() ) );
	if ( message.source () == kMessageSource_JS) {
		ofLogNotice( " (" + ToUTF8( message.source_id() ) + " @ line " + ofToString( message.line_number() ) + ", col " + ofToString( message.column_number() ) + ")" );
	}
	std::cout << std::endl;

}

//--------------------------------------------------------------
void ofxUltralight::update() {
	renderer->Update();
}

//--------------------------------------------------------------
void ofxUltralight::draw() {
	renderer->RefreshDisplay ( 0 );
	renderer->Render();

	// Get the Surface for the View (assuming CPU rendering)
	Surface* surface = view->surface ();

	// #### New: Prepare Custom Surface: paint directly into ofTexture
	GLPBOTextureSurface* texture_surface = ( GLPBOTextureSurface* ) surface;
	GLuint texture_id = texture_surface->GetTextureAndSyncIfNeeded ();

	// Check if the Surface is dirty (pixels have changed)
	if ( texture_surface->dirty_bounds( ).IsEmpty () ) {
		// #### Old: DEFAULT Surface implementation (BitmapSurface)
			// Cast to the default Surface implementation (BitmapSurface) and get
			// the underlying Bitmap.
			//RefPtr<Bitmap> bitmap = static_cast< BitmapSurface* >( surface )->bitmap ();

			//void* pixels = bitmap->LockPixels();

			///// Get the bitmap dimensions.
			//uint32_t width = bitmap->width();
			//uint32_t height = bitmap->height();
			//uint32_t stride = bitmap->row_bytes();

			//unsigned char* pixels2 = (unsigned char*)pixels;
			//// swap R and B channels (does nnot work in loadData later, don't know why.)
			//oeTexture.setSwizzle(GL_TEXTURE_SWIZZLE_R, GL_BLUE);
			//oeTexture.setSwizzle(GL_TEXTURE_SWIZZLE_B, GL_RED);
			//// load the pixels to ofTexture
			//oeTexture.loadData(pixels2, width, height, GL_RGBA);

			///// Unlock the Bitmap when we are done.
			//bitmap->UnlockPixels();

		// #### New: Custom Surface: paint directly into ofTexture
		// Preparation of Texture in setup!
		oeTexture.setUseExternalTextureID ( texture_id );
		/// Clear the dirty bounds.
		surface->ClearDirtyBounds ();
	}
	oeTexture.draw ( offset.x, offset.y );

}

//--------------------------------------------------------------
void ofxUltralight::keyPressed(int key) {
	int text = key;
	// get the correct virtual key codes
	int vk = key;
	switch (key) {
		case OF_KEY_LEFT:
			vk = KeyCodes::GK_LEFT;
			break;
		case OF_KEY_RIGHT:
			vk = KeyCodes::GK_RIGHT;
			break;
		case OF_KEY_UP:
			vk = KeyCodes::GK_UP;
			break;
		case OF_KEY_DOWN:
			vk = KeyCodes::GK_DOWN;
			break;
		case OF_KEY_DEL:
			vk = KeyCodes::GK_DELETE;
			break;
		case OF_KEY_BACKSPACE:
			vk = KeyCodes::GK_BACK;
			break;
			// this is the 'dot' key. For some reason I had to hard-code this in order to make it work
		case 46:
			vk = KeyCodes::GK_DECIMAL;
			break;
		case -1:
			break;

		case OF_KEY_RETURN:
			vk = KeyCodes::GK_RETURN;
			return;
		case OF_KEY_LEFT_SHIFT:
			vk = KeyCodes::GK_LSHIFT;
			return;
		case OF_KEY_RIGHT_SHIFT:
			vk = KeyCodes::GK_RSHIFT;
			return;
		case OF_KEY_LEFT_CONTROL:
			vk = KeyCodes::GK_LCONTROL;
			return;
		case OF_KEY_RIGHT_CONTROL:
			vk = KeyCodes::GK_RCONTROL;
			return;
		case OF_KEY_LEFT_ALT:
			return;
		case OF_KEY_RIGHT_ALT:
			return;
		case OF_KEY_LEFT_COMMAND:
			return;
		case OF_KEY_RIGHT_COMMAND:
			return;
		default:
			break;
	}

	KeyEvent evt;
	evt.type = KeyEvent::kType_RawKeyDown;
	// You'll need to generate a key identifier from the virtual key code
	// when synthesizing events. This function is provided in KeyEvent.h
	evt.virtual_key_code = vk;
	evt.native_key_code = key;
	evt.text = ofToString( (char)text ).c_str();
	evt.unmodified_text = ofToString( (char)text ).c_str();
	//evt.modifiers = (char)OF_KEY_MODIFIER;
	GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);
	view->FireKeyEvent(evt);

	KeyEvent evt2;
	evt2.type = KeyEvent::kType_Char;
	evt2.virtual_key_code = vk;
	evt2.native_key_code = key;
	evt2.text = ofToString( (char)text ).c_str();
	evt2.unmodified_text = ofToString( (char)text ).c_str();
	//evt2.modifiers = (char)OF_KEY_MODIFIER;
	evt2.unmodified_text = ofToString( (char)key ).c_str(); // If not available, set to same as evt.text

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
	ScrollEvent scroll_event;
	scroll_event.type = ScrollEvent::kType_ScrollByPixel;
	scroll_event.delta_x = scrollX * 30;
	scroll_event.delta_y = scrollY * 30;

	view->FireScrollEvent(scroll_event);
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

//--------------------------------------------------------------
string ofxUltralight::getStringFromJSstr(JSString str) {
	auto length = JSStringGetLength(str);
	auto buffer = new char[length];
	JSStringGetUTF8CString(str, buffer, length);
	
	return (string)buffer;
}
//--------------------------------------------------------------
void ofxUltralight::exit() {
}