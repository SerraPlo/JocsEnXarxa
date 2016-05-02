#pragma once
#include <GL/glew.h>

struct GLMesh {
	GLuint vao{ 0 };
	GLuint vbo{ 0 };
	GLuint ebo{ 0 };
	unsigned int elements{ 0 };

	GLMesh() = default;
	~GLMesh();
	void Load(const char*);
};


