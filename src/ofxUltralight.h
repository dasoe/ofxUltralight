#pragma once

/**************************************************************************************************
 *  This file uses Ultralight, an ultra-portable web-browser engine.                              *
 *                                                                                                *
 *  See <https://ultralig.ht> for licensing and more.                                             *
 *                                                                                                *
 *  (C) 2024 Ultralight, Inc.                                                                     *
 **************************************************************************************************/

#include <ofMain.h>
#include <Ultralight/Ultralight.h>
#include <JavaScriptCore/JavaScript.h>
#include <AppCore/Platform.h>
#include <AppCore/JSHelpers.h>



using namespace ultralight;




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

	void OnAddConsoleMessage(
		View* caller,
		const ConsoleMessage& message 
		);
		virtual void OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,const String& url);
		bool DOMready;

		void setup(int width, int height, string url);
		void setup(int width, int height, ofVec2f t_offset, string url, bool _transparent );
		void load(string url);
		void update();
		void draw();
		void exit();
		JSContextRef getJSContext();
		bool isDomReady();

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

///
/// Custom Surface implementation that allows Ultralight to paint directly
/// into an OpenGL PBO (pixel buffer object).
///
/// PBOs in OpenGL allow us to get a pointer to a block of GPU-controlled
/// memory for lower-latency uploads to a texture.
///
/// For more info: <http://www.songho.ca/opengl/gl_pbo.html>
///
class GLPBOTextureSurface : public Surface {
public:
    GLPBOTextureSurface ( uint32_t width, uint32_t height ) {
        Resize ( width, height );
    }

    virtual ~GLPBOTextureSurface () {
        ///
        /// Destroy our PBO and texture.
        ///
        if ( pbo_id_ ) {
            glDeleteBuffers ( 1, &pbo_id_ );
            pbo_id_ = 0;
            glDeleteTextures ( 1, &texture_id_ );
            texture_id_ = 0;
        }
    }

    virtual uint32_t width () const override { return width_; }

    virtual uint32_t height () const override { return height_; }

    virtual uint32_t row_bytes () const override { return row_bytes_; }

    virtual size_t size () const override { return size_; }

    virtual void* LockPixels () override {
        ///
        /// Map our PBO to system memory so Ultralight can draw to it.
        ///
        glBindBuffer ( GL_PIXEL_UNPACK_BUFFER, pbo_id_ );
        void* result = glMapBuffer ( GL_PIXEL_UNPACK_BUFFER, GL_READ_WRITE );
        glBindBuffer ( GL_PIXEL_UNPACK_BUFFER, 0 );
        return result;
    }

    virtual void UnlockPixels () override {
        ///
        /// Unmap our PBO.
        ///
        glBindBuffer ( GL_PIXEL_UNPACK_BUFFER, pbo_id_ );
        glUnmapBuffer ( GL_PIXEL_UNPACK_BUFFER );
        glBindBuffer ( GL_PIXEL_UNPACK_BUFFER, 0 );
    }

    virtual void Resize ( uint32_t width, uint32_t height ) override {
        if ( pbo_id_ && width_ == width && height_ == height )
            return;

        ///
        /// Destroy any existing PBO and texture.
        ///
        if ( pbo_id_ ) {
            glDeleteBuffers ( 1, &pbo_id_ );
            pbo_id_ = 0;
            glDeleteTextures ( 1, &texture_id_ );
            texture_id_ = 0;
        }

        width_ = width;
        height_ = height;
        row_bytes_ = width_ * 4;
        size_ = row_bytes_ * height_;

        ///
        /// Create our PBO (pixel buffer object), with a size of 'size_'
        ///
        glGenBuffers ( 1, &pbo_id_ );
        glBindBuffer ( GL_PIXEL_UNPACK_BUFFER, pbo_id_ );
        glBufferData ( GL_PIXEL_UNPACK_BUFFER, size_, 0, GL_DYNAMIC_DRAW );
        glBindBuffer ( GL_PIXEL_UNPACK_BUFFER, 0 );

        ///
        /// Create our Texture object.
        ///
        glGenTextures ( 1, &texture_id_ );
        glBindTexture ( GL_TEXTURE_2D, texture_id_ );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
        glBindTexture ( GL_TEXTURE_2D, 0 );
    }

    virtual GLuint GetTextureAndSyncIfNeeded () {
        ///
        /// This is NOT called by Ultralight.
        ///
        /// This helper function is called when our application wants to draw this
        /// Surface to an OpenGL quad. (We return an OpenGL texture handle)
        ///
        /// We take this opportunity to upload the PBO to the texture if the
        /// pixels have changed since the last call (indicated by dirty_bounds()
        /// being non-empty)
        ///
        if ( !dirty_bounds ().IsEmpty () ) {
            ///
            /// Update our Texture from our PBO (pixel buffer object)
            ///
            glBindTexture ( GL_TEXTURE_2D, texture_id_ );
            glBindBuffer ( GL_PIXEL_UNPACK_BUFFER, pbo_id_ );
            glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, width_, height_,
                0, GL_BGRA, GL_UNSIGNED_BYTE, 0 );
            glBindBuffer ( GL_PIXEL_UNPACK_BUFFER, 0 );
            glBindTexture ( GL_TEXTURE_2D, 0 );

            ///
            /// Clear our Surface's dirty bounds to indicate we've handled any
            /// pending modifications to our pixels.
            ///
            ClearDirtyBounds ();
        }

        return texture_id_;
    }

protected:
    GLuint texture_id_;
    GLuint pbo_id_ = 0;
    uint32_t width_;
    uint32_t height_;
    uint32_t row_bytes_;
    uint32_t size_;
};

class GLTextureSurfaceFactory : public ultralight::SurfaceFactory {
public:
    GLTextureSurfaceFactory () {}

    virtual ~GLTextureSurfaceFactory () {}

    virtual ultralight::Surface* CreateSurface ( uint32_t width, uint32_t height ) override {
        ///
        /// Called by Ultralight when it wants to create a Surface.
        ///
        return new GLPBOTextureSurface ( width, height );
    }

    virtual void DestroySurface ( ultralight::Surface* surface ) override {
        //
        /// Called by Ultralight when it wants to destroy a Surface.
        ///
        delete static_cast< GLPBOTextureSurface* >( surface );
    }
};