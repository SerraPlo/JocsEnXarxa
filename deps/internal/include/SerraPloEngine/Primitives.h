#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"

namespace SerraPlo {

	class DebugCube {
	public:
		GLuint vao, vbo, textureID{0};
		glm::vec3 position{ 0,0,0 };
		DebugCube();
		void Draw(ShaderProgram &program);
	};

	class GLText {
	public:
		GLuint textureid;
		GLuint vao, vbo;
		glm::vec3 position{ 0,0,0 };
		glm::vec3 rotation{ 0,0,0 };
		glm::vec3 scale{ 0,0,0 };
		std::string message{" "};
		GLText();
		~GLText() = default;
		void Draw(ShaderProgram &program, TTF_Font *font);
	};

}


