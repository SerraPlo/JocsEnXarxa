#pragma once
#include <string>
#include <SDL2\SDL.h>
#include "Vertex.h"

namespace SerraPlo {
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window {
	protected:
		SDL_Window* m_SDLWindow = nullptr;
		std::string m_engineName = "";
		int m_screenWidth = 600;
		int m_screenHeight = 600;
	public:
		explicit Window() = default;
		explicit Window(int sw, int sh, const std::string &name = "");
		virtual ~Window();
		virtual void create(const ColorRGBA8& c = ColorRGBA8{ 255, 255, 255, 255 }, const unsigned &curFlags = 0);
		virtual void create(const std::string &name = "", int sw = 600, int sh = 600, const unsigned &curFlags = 0);
		void changeSize(const int &sw, const int &sh) const { SDL_SetWindowSize(m_SDLWindow, sw, sh); };
		SDL_Window* getWindow() const { return m_SDLWindow; };
		void swapBuffer() const { SDL_GL_SwapWindow(m_SDLWindow); };
		int getScreenWidth() const { return m_screenWidth; };
		int getScreenHeight() const { return m_screenHeight; };
	};

	class GLWindow : public Window {
		SDL_GLContext m_glContext = nullptr;
	public:
		explicit GLWindow() = default;
		explicit GLWindow(int sw, int sh, const std::string &name = "");
		~GLWindow();
		void create(const ColorRGBA8& c = ColorRGBA8{ 255, 255, 255, 255 }, const unsigned &curFlags = 0) override;
		void create(const std::string &name = "", int sw = 600, int sh = 600, const unsigned &curFlags = 0) override;
	};
}