#include "Window.h"
#include "ErrorManager.h"
#include <GL/glew.h>

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
	}

	void GLWindow::create(const std::string & name, int sw, int sh, const unsigned & curFlags) {
		m_engineName = name;
		m_screenWidth = sw;
		m_screenHeight = sh;
		create(curFlags);
	}
}
