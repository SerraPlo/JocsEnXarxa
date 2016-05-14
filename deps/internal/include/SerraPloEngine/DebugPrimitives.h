#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"

namespace SerraPlo {
	class DebugCube {
	public:
		GLuint vao, vbo, textureID;
		glm::vec3 position;
		DebugCube();
		void Draw(ShaderProgram &program);
	};

	class DebugText {
	public:
		GLuint vao, vbo;
		glm::vec3 position;
		std::string message{"-"};
		DebugText();
		void Draw(ShaderProgram &program);
	};
}


