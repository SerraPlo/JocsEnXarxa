#pragma once
#include <GL/glew.h>

namespace SerraPlo {

	struct GLMesh {
		GLuint vao{ 0 };
		GLuint vbo{ 0 };
		GLuint ebo{ 0 };
		unsigned int elements{ 0 };
		explicit GLMesh(const char* filePath);
		//~GLMesh();  //TODO
	};

}
