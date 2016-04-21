#pragma once
#include <GL\glew.h>
#pragma comment(lib, "glew32s.lib")

namespace SerraPlo {

	struct Position {
		GLfloat x, y;
		//explicit Position() = default;
	};

	struct UV {
		GLfloat u, v;
		//explicit UV() = default;
	};

	struct ColorRGBA8 {
		GLubyte r, g, b, a;
		//explicit ColorRGBA8() = default;
	};

	struct Vertex {
		Position position;
		UV uv;
		ColorRGBA8 color;
		//explicit Vertex() = default;
	};

}