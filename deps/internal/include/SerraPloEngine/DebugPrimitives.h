#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <SDL2/SDL_ttf.h>
#pragma comment(lib, "SDL2_ttf.lib")

namespace SerraPlo {
	class DebugCube {
	public:
		GLuint vao, vbo, textureID{0};
		glm::vec3 position{ 0,0,0 };
		DebugCube();
		void Draw(ShaderProgram &program);
	};

	class DebugText {
	public:
		TTF_Font *font;
		GLuint vao, vbo;
		glm::vec3 position{ 0,0,0 };
		std::string message{" "};
		DebugText();
		~DebugText();
		void Draw(ShaderProgram &program);
	};
}

