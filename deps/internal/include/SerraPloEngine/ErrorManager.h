#pragma once
#include <string>
#include <SDL2\SDL.h>

namespace SerraPlo {
	#define ThrowError(message) fatalError(message, __FILE__, __LINE__)
	extern void fatalError(const std::string &errorString, const std::string& fileName, int line) {
		SDL_Quit();
		throw std::exception(errorString.c_str());
	}
}


