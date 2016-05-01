#pragma once
#include <glm/glm.hpp>
#include "Vertex.h"
#include <vector>

namespace SerraPlo {

	class Renderer3D {
		GLuint m_vbo{ 0 }; // vertex buffer object
		GLuint m_vao{ 0 }; // vertex array object
		GLuint m_ebo{ 0 }; // element buffer objects

	public:
		Renderer3D();
		~Renderer3D();

		void Init();

		void Begin();
		void Render();
		void End();
	};

}



