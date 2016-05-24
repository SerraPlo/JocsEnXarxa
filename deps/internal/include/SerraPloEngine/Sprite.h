#pragma once
#include <glm/glm.hpp>
#include <string>
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_image.h>
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_ttf.lib")
#include "InputManager.h"
#include <iostream>

namespace SerraPlo {

	class Text {
	public:
		SDL_Texture *texture{ nullptr };
		glm::ivec2 position;
		int width, height;
		Text() = default;
		virtual ~Text() { SDL_DestroyTexture(texture); }
		void Load(const std::string &msg, const SDL_Color &color, SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font) {
			if (!msg.empty()) {
				SDL_Surface *textureSurface = TTF_RenderText_Blended(font, msg.c_str(), color);
				texture = SDL_CreateTextureFromSurface(renderer, textureSurface);
				width = textureSurface->w;
				height = textureSurface->h;
				SDL_FreeSurface(textureSurface);
			}
		}
		void Draw(SDL_Renderer *renderer) {
			SDL_Rect srcRect = { 0, 0, width, height };
			SDL_Rect destRect = { position.x, position.y, width, height };
			SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
		}
	};

	class InputText : public Text {
	public:
		InputText() = default;
		~InputText() = default;
		void Draw(SDL_Renderer *renderer, int characterSize) {
			SDL_Rect srcRect = { 0, 0, characterSize, height };
			SDL_Rect destRect = { position.x, position.y, characterSize, height };
			SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
		}
	};

	class Sprite {
	public:
		SDL_Texture *texture{ nullptr };
		glm::ivec2 position;
		int width, height;
		Sprite() = default;
		virtual ~Sprite() { SDL_DestroyTexture(texture); }
		void Load(const std::string &path, SDL_Window *window, SDL_Renderer *renderer) {
			SDL_Surface *loadSurface = IMG_Load(path.c_str());
			SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
			SDL_Surface *textureSurface = SDL_ConvertSurface(loadSurface, screenSurface->format, NULL);
			texture = SDL_CreateTextureFromSurface(renderer, textureSurface);
			width = textureSurface->w;
			height = textureSurface->h;
			SDL_FreeSurface(loadSurface);
			SDL_FreeSurface(textureSurface);
		}
		void Draw(SDL_Renderer *renderer) {
			SDL_Rect srcRect = { 0, 0, width, height };
			SDL_Rect destRect = { position.x, position.y, width, height };
			SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
		}
	};

	class Button : public Sprite {
	public:
		bool pressed{ false };
		bool hovered{ false };
		Button() = default;
		~Button() = default;
		void Update(InputManager &inputManager) {
			hovered = false;
			pressed = false;
			if (inputManager.mouseCoords.x > position.x && inputManager.mouseCoords.x < position.x + width &&
				inputManager.mouseCoords.y > position.y && inputManager.mouseCoords.y < position.y + height) {
				hovered = true;
				if (inputManager.isKeyPressed(SDL_BUTTON_LEFT)) pressed = true;
			}
		}
	};

}