#pragma once
#include <glm/glm.hpp>
#include <string>
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#pragma comment(lib, "SDL2_image.lib")
#include "InputManager.h"

namespace SerraPlo {

	class Sprite2D {
	public:
		SDL_Texture *texture{ nullptr };
		glm::ivec2 position;
		int width, height;
		Sprite2D() = default;
		virtual ~Sprite2D() { SDL_DestroyTexture(texture); }
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
		virtual void Draw(SDL_Renderer *renderer) {
			SDL_Rect srcRect = { 0, 0, width, height };
			SDL_Rect destRect = { position.x, position.y, width, height };
			SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
		}
	};

	class Button2D : public Sprite2D {
	public:
		SDL_Texture *textureHover{ nullptr };
		bool hover{ false };
		bool pressed{ false };
		Button2D() = default;
		~Button2D() = default;
		void Reset() {
			hover = false;
			pressed = false;
		}
		void Load(const std::string &pathDefault, const std::string &pathHover, SDL_Window *window, SDL_Renderer *renderer) {
			SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
			SDL_Surface *loadSurface = IMG_Load(pathDefault.c_str());
			SDL_Surface *textureSurface = SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_ARGB8888, 0);
			texture = SDL_CreateTextureFromSurface(renderer, textureSurface);
			loadSurface = IMG_Load(pathHover.c_str());
			textureSurface = SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_ARGB8888, 0);
			textureHover = SDL_CreateTextureFromSurface(renderer, textureSurface);
			width = textureSurface->w;
			height = textureSurface->h;
			SDL_FreeSurface(loadSurface);
			SDL_FreeSurface(textureSurface);
		}
		void Update(InputManager &inputManager) {
			hover = false;
			if (inputManager.mouseCoords.x > position.x && inputManager.mouseCoords.x < position.x + width &&
				inputManager.mouseCoords.y > position.y && inputManager.mouseCoords.y < position.y + height) {
				hover = true;
				if (inputManager.isKeyPressed(SDL_BUTTON_LEFT)) pressed = true;
			}
		}
		void Draw(SDL_Renderer *renderer) override {
			SDL_Rect srcRect = { 0, 0, width, height };
			SDL_Rect destRect = { position.x, position.y, width, height };
			if (hover && textureHover != nullptr) SDL_RenderCopy(renderer, textureHover, &srcRect, &destRect);
			else SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
		}
	};
}