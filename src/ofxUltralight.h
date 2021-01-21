#pragma once

#include <ofMain.h>
#include <Ultralight/Ultralight.h>
#include <JavaScriptCore/JavaScript.h>
#include <AppCore/Platform.h>
#include <AppCore/JSHelpers.h>



using namespace ultralight;

// Define our custom Logger class
class MyLogger : public Logger {
public:
	MyLogger() {}
	virtual ~MyLogger() {}
	/// Called when the library wants to print a message to the log.
	virtual void LogMessage(LogLevel log_level, const String16& message) override {
		// give out the message via OF Logging
		if (log_level == kLogLevel_Error) {
			ofLogError( ofToString("[Ultralight]: ") + String(message).utf8().data() );
		} else if (log_level == kLogLevel_Warning) {
			ofLogWarning(ofToString("[Ultralight]: ") + String(message).utf8().data() );
		} else {
			ofLogNotice(ofToString("[Ultralight]: ") + String(message).utf8().data() );
		}

	}

	inline std::string ToUTF8(const String& str) {
  String8 utf8 = str.utf8();
  return std::string(utf8.data(), utf8.length());
}

inline const char* Stringify(MessageSource source) {
  switch(source) {
    case kMessageSource_XML: return "XML";
    case kMessageSource_JS: return "JS";
    case kMessageSource_Network: return "Network";
    case kMessageSource_ConsoleAPI: return "ConsoleAPI";
    case kMessageSource_Storage: return "Storage";
    case kMessageSource_AppCache: return "AppCache";
    case kMessageSource_Rendering: return "Rendering";
    case kMessageSource_CSS: return "CSS";
    case kMessageSource_Security: return "Security";
    case kMessageSource_ContentBlocker: return "ContentBlocker";
    case kMessageSource_Other: return "Other";
    default: return "";
  }
}

inline const char* Stringify(MessageLevel level) {
  switch(level) {
    case kMessageLevel_Log: return "Log";
    case kMessageLevel_Warning: return "Warning";
    case kMessageLevel_Error: return "Error";
    case kMessageLevel_Debug: return "Debug";
    case kMessageLevel_Info: return "Info";
    default: return "";
  }
}


};



class ofxUltralight : public LoadListener, public ViewListener
	{

public:


	inline std::string ToUTF8(const String& str) {
		String8 utf8 = str.utf8();
		return std::string(utf8.data(), utf8.length());
	}

	inline const char* Stringify(MessageSource source) {
		switch (source) {
		case kMessageSource_XML: return "XML";
		case kMessageSource_JS: return "JS";
		case kMessageSource_Network: return "Network";
		case kMessageSource_ConsoleAPI: return "ConsoleAPI";
		case kMessageSource_Storage: return "Storage";
		case kMessageSource_AppCache: return "AppCache";
		case kMessageSource_Rendering: return "Rendering";
		case kMessageSource_CSS: return "CSS";
		case kMessageSource_Security: return "Security";
		case kMessageSource_ContentBlocker: return "ContentBlocker";
		case kMessageSource_Other: return "Other";
		default: return "";
		}
	}

	inline const char* Stringify(MessageLevel level) {
		switch (level) {
		case kMessageLevel_Log: return "Log";
		case kMessageLevel_Warning: return "Warning";
		case kMessageLevel_Error: return "Error";
		case kMessageLevel_Debug: return "Debug";
		case kMessageLevel_Info: return "Info";
		default: return "";
		}
	}

	void OnAddConsoleMessage(View* caller,
		MessageSource source,
		MessageLevel level,
		const String& message,
		uint32_t line_number,
		uint32_t column_number,
		const String& source_id);
		virtual void OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,const String& url);
		bool DOMready;

		void setup(int width, int height, string url);
		void setup(int width, int height, ofVec2f t_offset, string url);
		void load(string url);
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		string getStringFromJSstr(JSString str);

		Config config;
		shared_ptr<GPUDriver> gpu_driver;
		RefPtr<Renderer> renderer;
		RefPtr<View> view, inspectorView;
		JSContextRef jsContext;

		ofVec2f offset;
		ofTexture oeTexture, inspectorTexture;
};
