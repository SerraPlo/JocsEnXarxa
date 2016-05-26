#pragma once
#include <string>
#include <SDL2\SDL.h>
#include "InputManager.h"

namespace SerraPlo {

	class Sprite2D {
	public:
		SDL_Texture *texture{ nullptr };
		glm::ivec2 position;
		int width, height;
		Sprite2D() = default;
		virtual ~Sprite2D() { SDL_DestroyTexture(texture); }
		void Load(int x, int y, int w, int h, const std::string &path, SDL_Window *window, SDL_Renderer *renderer);
		virtual void Draw(SDL_Renderer *renderer);
	};

	class Button2D : public Sprite2D {
	public:
		SDL_Texture *textureHover{ nullptr };
		bool hover{ false };
		bool pressed{ false };
		Button2D() = default;
		~Button2D() { SDL_DestroyTexture(texture), SDL_DestroyTexture(textureHover); }
		void Reset() { hover = false; pressed = false; }
		void Load(int x, int y, int w, int h, const std::string &pathDefault, const std::string &pathHover, SDL_Window *window, SDL_Renderer *renderer);
		void Update(InputManager &inputManager);
		void Draw(SDL_Renderer *renderer) override;
	};
}