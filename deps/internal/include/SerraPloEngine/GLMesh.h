#pragma once
#include <GL/glew.h>

namespace SerraPlo {

	struct GLMesh {
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		unsigned int elements;
		explicit GLMesh() = default;
		explicit GLMesh(const char* filePath);
		//~GLMesh();  //TODO
	};

}
