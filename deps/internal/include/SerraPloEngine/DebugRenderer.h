#pragma once
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Vertex.h"
#include <vector>

namespace SerraPlo {

	class DebugRenderer {
	public:
		explicit DebugRenderer() = default;
		~DebugRenderer();

		void init();
		void end();
		void drawBox(const glm::vec4& destRect, const ColorRGBA8 &color, float angle);
		void drawCircle(const glm::vec2& center, const ColorRGBA8 &color, float radius);
		void render(const glm::mat4 &projectionMatrix, float lineWidth) const;
		void dispose() const;

		struct DebugVertex {
			glm::vec2 position;
			ColorRGBA8 color;
		};
	private:
		ShaderProgram m_program;
		std::vector<DebugVertex> m_vertices;
		std::vector<GLuint> m_indices;
		GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;
		int m_numElements = 0;
	};

}
