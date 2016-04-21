#pragma once
#include <GL\glew.h>
#pragma comment(lib, "glew32s.lib")

namespace SerraPlo {
	struct GLTexture {
		GLuint id;
		int width;
		int height;
	};
}

