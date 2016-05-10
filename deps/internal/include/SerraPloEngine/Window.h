#pragma once
#include <string>
#include <SDL2\SDL.h>

namespace SerraPlo {
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4, RESIZABLE = 0x8 };

	class Window {
	protected:
		std::string m_engineName = "";
	public:
		SDL_Window* SDLWindow = nullptr;
		int *screenWidth = nullptr;
		int *screenHeight = nullptr;
		explicit Window() = default;
		explicit Window(int *sw, int *sh, const std::string &name = "");
		virtual ~Window();
		virtual void create(const unsigned &curFlags = 0);
		virtual void create(const std::string &name, int *sw, int *sh, const unsigned &curFlags = 0);
		void changeSize(int sw, int sh) const { SDL_SetWindowSize(SDLWindow, sw, sh); };
		void changeName(const std::string &name) const { SDL_SetWindowTitle(SDLWindow, name.c_str()); };
		void swapBuffer() const { SDL_GL_SwapWindow(SDLWindow); };
	};

	class GLWindow : public Window {
		SDL_GLContext m_glContext = nullptr;
	public:
		explicit GLWindow() = default;
		explicit GLWindow(int *sw, int *sh, const std::string &name = "");
		~GLWindow();
		void create(const unsigned &curFlags = 0) override;
		void create(const std::string &name, int *sw, int *sh, const unsigned &curFlags = 0) override;
	};
}