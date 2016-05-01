#include "Window.h"
#include "ErrorManager.h"

namespace SerraPlo {

	//========================================================
	//						WINDOW
	//========================================================

	Window::Window(int sw, int sh, const std::string &name) :
		m_SDLWindow(nullptr),
		m_engineName(name),
		m_screenWidth(sw),
		m_screenHeight(sh)
	{};

	Window::~Window() {
		if (m_SDLWindow != nullptr) SDL_DestroyWindow(m_SDLWindow);
		SDL_Quit();
	};

	void Window::create(const unsigned &curFlags) {
		Uint32 flags = 0;
		if (curFlags & INVISIBLE) flags |= SDL_WINDOW_HIDDEN;
		else if (curFlags & FULLSCREEN) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		else if (curFlags & BORDERLESS) flags |= SDL_WINDOW_BORDERLESS;
		else if (curFlags & RESIZABLE) flags |= SDL_WINDOW_RESIZABLE;
		m_SDLWindow = SDL_CreateWindow(m_engineName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, flags);
		if (m_SDLWindow == nullptr) SP_THROW_ERROR("SDL Window could not be created.");
	}

	void Window::create(const std::string & name, int sw, int sh, const unsigned & curFlags) {
		m_engineName = name;
		m_screenWidth = sw;
		m_screenHeight = sh;
		create(curFlags);
	}

	//========================================================
	//						GLWINDOW
	//========================================================

	GLWindow::GLWindow(int sw, int sh, const std::string &name) :
		Window(sw, sh, name),
		m_glContext(nullptr)
	{};

	GLWindow::~GLWindow() {
		if (m_SDLWindow != nullptr) SDL_DestroyWindow(m_SDLWindow);
		if (m_glContext != nullptr) SDL_GL_DeleteContext(m_glContext);
		SDL_Quit();
	};

	void GLWindow::EnableGLHint() {
		//Indicates the accuracy of fog calculation.If per - pixel fog calculation is not efficiently supported by the GL implementation, 
		//hinting GL_DONT_CARE or GL_FASTEST can result in per - vertex calculation of fog effects.	
		glHint(GL_FOG_HINT, GL_NICEST);
		//Indicates the accuracy of the derivative calculation for the GL shading language fragment processing built - in functions : dFdx, dFdy, and fwidth.
		glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
		//Indicates the quality of filtering when generating mipmap images.
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		//Indicates the sampling quality of antialiased lines.If a larger filter function is applied, hinting GL_NICEST can result in more pixel fragments being generated during rasterization.
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		//Indicates the quality of color, texture coordinate, and fog coordinate interpolation.If perspective - corrected parameter interpolation is not efficiently supported by the GL implementation, 
		//hinting GL_DONT_CARE or GL_FASTEST can result in simple linear interpolation of colors and / or texture coordinates.
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		//Indicates the sampling quality of antialiased points.If a larger filter function is applied, hinting GL_NICEST can result in more pixel fragments being generated during rasterization.
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		//Indicates the sampling quality of antialiased polygons.Hinting GL_NICEST can result in more pixel fragments being generated during rasterization, if a larger filter function is applied.
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		//Indicates the quality and performance of the compressing texture images. Hinting GL_FASTEST indicates that texture images should be compressed as quickly as possible, 
		//while GL_NICEST indicates that texture images should be compressed with as little image quality loss as possible. 
		//GL_NICEST should be selected if the texture is to be retrieved by glGetCompressedTexImage for reuse.
		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
	}

	void GLWindow::create(const unsigned &curFlags) {
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (curFlags & INVISIBLE) flags |= SDL_WINDOW_HIDDEN;
		else if (curFlags & FULLSCREEN) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		else if (curFlags & BORDERLESS) flags |= SDL_WINDOW_BORDERLESS;
		else if (curFlags & RESIZABLE) flags |= SDL_WINDOW_RESIZABLE;

		m_SDLWindow = SDL_CreateWindow(m_engineName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, flags);
		if (m_SDLWindow == nullptr) SP_THROW_ERROR("SDL Window could not be created.");

		m_glContext = SDL_GL_CreateContext(m_SDLWindow);
		if (m_glContext == nullptr) SP_THROW_ERROR("SDL_GL Context could not be created.");

		glewExperimental = true;
		if (glewInit() != GLEW_OK) SP_THROW_ERROR("GLEW could not be initialized.");

		printf("***  OpenGL Version: %s  ***\n", reinterpret_cast<char const*>(glGetString(GL_VERSION)));

		SDL_GL_SetSwapInterval(0);	// Set V-Sync
		glClearDepth(1.0);			// Set the base depth when depth buffer
		glEnable(GL_DEPTH_TEST);	// Activate the Z-buffer
		glEnable(GL_BLEND);			// Enable alpha blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		EnableGLHint();
	}

	void GLWindow::create(const std::string & name, int sw, int sh, const unsigned & curFlags) {
		m_engineName = name;
		m_screenWidth = sw;
		m_screenHeight = sh;
		create(curFlags);
	}
}