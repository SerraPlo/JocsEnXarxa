#pragma once
#include <glm/glm.hpp>
#include <GL\glew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

namespace SerraPlo {

	struct Position2D {
		GLfloat x, y;
		explicit Position2D() = default;
	};

	struct Normal {
		GLfloat x, y, z;
		explicit Normal() = default;
	};

	struct UV {
		GLfloat u, v;
		explicit UV() = default;
	};

	struct ColorRGBA8 {
		GLubyte r, g, b, a;
		explicit ColorRGBA8() = default;
	};

	struct Vertex2D {
		Position2D position;
		UV uv;
		ColorRGBA8 color;
		explicit Vertex2D() = default;
	};

	struct Vertex3D {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
		explicit Vertex3D() = default;
	};

}