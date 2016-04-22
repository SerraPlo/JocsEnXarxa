#pragma once
#include <GL\glew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

namespace SerraPlo {
	struct GLTexture {
		GLuint id;
		int width;
		int height;
	};
}

