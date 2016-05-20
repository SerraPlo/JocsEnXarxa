#pragma once
#include <GL\glew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#pragma comment(lib, "SDL2_ttf.lib")
#include <functional>
#include "ResourceManager.h"
#include <iostream>
#include "Window.h"

namespace SerraPlo {

#define EnableGLHint() \
		/*Indicates the accuracy of fog calculation.If per - pixel fog calculation is not efficiently supported by the GL implementation, 
		hinting GL_DONT_CARE or GL_FASTEST can result in per - vertex calculation of fog effects. */ \
		glHint(GL_FOG_HINT, GL_NICEST); \
		/*Indicates the accuracy of the derivative calculation for the GL shading language fragment processing built - in functions : dFdx, dFdy, and fwidth.*/ \
		glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST); \
		/*Indicates the quality of filtering when generating mipmap images.*/ \
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST); \
		/*Indicates the sampling quality of antialiased lines.If a larger filter function is applied, hinting GL_NICEST can result in more pixel fragments being generated during rasterization.*/ \
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); \
		/*Indicates the quality of color, texture coordinate, and fog coordinate interpolation.If perspective - corrected parameter interpolation is not efficiently supported by the GL implementation,
		hinting GL_DONT_CARE or GL_FASTEST can result in simple linear interpolation of colors and / or texture coordinates.*/ \
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); \
		/*Indicates the sampling quality of antialiased points.If a larger filter function is applied, hinting GL_NICEST can result in more pixel fragments being generated during rasterization.*/ \
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); \
		/*Indicates the sampling quality of antialiased polygons.Hinting GL_NICEST can result in more pixel fragments being generated during rasterization, if a larger filter function is applied.*/ \
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); \
		/*Indicates the quality and performance of the compressing texture images. Hinting GL_FASTEST indicates that texture images should be compressed as quickly as possible, 
		while GL_NICEST indicates that texture images should be compressed with as little image quality loss as possible. 
		GL_NICEST should be selected if the texture is to be retrieved by glGetCompressedTexImage for reuse.*/ \
		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST); \
	
	// Initialize SDL systems
	void InitSDL(void) {
		SDL_Init(SDL_INIT_EVERYTHING);	// Initialize everything in SDL (VIDEO, AUDIO, EVENTS,...)
		TTF_Init();						// Initialize SDL_ttf for text purposes
		// Tell we want a double buffered windows to avoid flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	}

	// Initialize SDL attributes
	void InitOpenGL(bool enableGLHint = true) {
		glClearDepth(1.0);			// Set the base depth when depth buffer
		glEnable(GL_DEPTH_TEST);	// Activate the Z-buffer
		glEnable(GL_BLEND);			// Enable alpha blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glLineWidth(5.0f);
		if (enableGLHint) EnableGLHint();
	}

	// Create a message box in which to ask the user whether to play on fullscreen or windowed mode
	int AskUserForWindow(void) {
		const SDL_MessageBoxButtonData buttons[] = { { 0, 0, "NO" }, { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "YES" } };
		const SDL_MessageBoxColorScheme colorScheme = { { { 255,   0,   0 }, { 0, 255,   0 }, { 255, 255,   0 }, { 0,   0, 255 }, { 255,   0, 255 } } };
		const SDL_MessageBoxData messageBoxData = { SDL_MESSAGEBOX_INFORMATION, nullptr, "Select an option", "Do you want to play on full screen mode?", SDL_arraysize(buttons), buttons, &colorScheme };
		int buttonID;
		SDL_ShowMessageBox(&messageBoxData, &buttonID); // Whether to play on fullscreen mode or default normal mode
		return buttonID;
	}

#define TEXT_WIDTH 700
#define TEXT_HEIGHT 100
#define TEXT_MAX_SIZE 10
#define FONT_SIZE 70

	std::string GetUserNick(GLWindow &window) {
		SDL_Renderer *msgboxR = SDL_CreateRenderer(window.SDLWindow, 0, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(msgboxR, 255, 255, 255, 255);
		TTF_Font *font = TTF_OpenFont(LoadAsset("fonts/ARIAL.TTF").c_str(), FONT_SIZE);
		SDL_StartTextInput();
		std::string text = "";
		SDL_Surface *text_surface = nullptr;
		SDL_Texture *texture = nullptr;
		SDL_bool done = SDL_FALSE;
		while (!done) {
			SDL_RenderClear(msgboxR);
			text_surface = TTF_RenderText_Blended(font, "Enter your nickname: ", { 0,0,0 });
			texture = SDL_CreateTextureFromSurface(msgboxR, text_surface);
			SDL_RenderCopy(msgboxR, texture, &SDL_Rect{ 0 , 0, TEXT_WIDTH, TEXT_HEIGHT },
						   &SDL_Rect{ *window.screenWidth / 2 - TEXT_WIDTH / 2 , *window.screenHeight / 2 - TEXT_HEIGHT / 2 - 100, TEXT_WIDTH, TEXT_HEIGHT });
			SDL_DestroyTexture(texture);
			SDL_FreeSurface(text_surface);

			text_surface = TTF_RenderText_Blended(font, text.c_str(), { 100,0,100 });
			texture = SDL_CreateTextureFromSurface(msgboxR, text_surface);
			SDL_RenderCopy(msgboxR, texture, &SDL_Rect{ 0, 0, int(FONT_SIZE*text.size()), TEXT_HEIGHT },
						   &SDL_Rect{ *window.screenWidth / 2 - TEXT_WIDTH / 2 , *window.screenHeight / 2 - TEXT_HEIGHT / 2 + 100, int(FONT_SIZE * text.size()), TEXT_HEIGHT });
			SDL_RenderPresent(msgboxR);
			SDL_DestroyTexture(texture);
			SDL_FreeSurface(text_surface);

			SDL_Event event;
			if (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT:
					exit(EXIT_SUCCESS);
					case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_RETURN && !text.empty()) done = SDL_TRUE;
					if (event.key.keysym.sym == SDLK_BACKSPACE) text = text.substr(0, text.size() - 1);
					if (event.key.keysym.sym == SDLK_ESCAPE) exit(EXIT_SUCCESS);
					break;
					case SDL_TEXTINPUT:
					if (text.size() < TEXT_MAX_SIZE) text += event.text.text;
					break;
					case SDL_TEXTEDITING:
					break;
				}
			}
		}
		SDL_StopTextInput();
		SDL_DestroyRenderer(msgboxR);
		TTF_CloseFont(font);
		return text;
	}

	void SetLoadingScreen(GLWindow &window, std::function<void(void)> loadFunction) {
		SDL_Renderer *msgboxR = SDL_CreateRenderer(window.SDLWindow, 0, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(msgboxR, 255, 255, 255, 255);
		SDL_Surface *text_surface = nullptr;
		SDL_RenderClear(msgboxR);
		TTF_Font *font = TTF_OpenFont(LoadAsset("fonts/ARIAL.TTF").c_str(), FONT_SIZE);
		text_surface = TTF_RenderText_Blended(font, "Loading...", { 0,0,0 });
		SDL_Texture *texture = SDL_CreateTextureFromSurface(msgboxR, text_surface);
		SDL_RenderCopy(msgboxR, texture, &SDL_Rect{ 0 , 0, TEXT_WIDTH, TEXT_HEIGHT },
					   &SDL_Rect{ *window.screenWidth / 2 - TEXT_WIDTH / 2 , *window.screenHeight / 2 - TEXT_HEIGHT / 2, TEXT_WIDTH, TEXT_HEIGHT });
		SDL_RenderPresent(msgboxR);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(text_surface);
		//-------------
		loadFunction();
		//-------------
		SDL_DestroyRenderer(msgboxR);
		TTF_CloseFont(font);
	}

}
