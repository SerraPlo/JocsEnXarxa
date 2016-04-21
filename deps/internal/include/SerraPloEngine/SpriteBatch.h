#pragma once
#include <glm/glm.hpp>
#include "Vertex.h"
#include <vector>

namespace SerraPlo {

	enum class GlyphSortType { NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE };

	class Glyph {
		glm::vec2 RotatePoint(glm::vec2 pos, float angle) const;
	public:
		GLuint texture;
		float depth;
		Vertex topLeft, bottomLeft, topRight, bottomRight;

		explicit Glyph() = default;
		explicit Glyph(GLuint texture_, float depth_, const glm::vec4 & destRect, const glm::vec4 & uvRect, const ColorRGBA8 &color);
		explicit Glyph(GLuint texture_, float depth_, const glm::vec4 & destRect, const glm::vec4 & uvRect, const ColorRGBA8 &color, float angle);
	};

	struct RenderBatch {
		GLuint offset, numVertices, texture;
		explicit RenderBatch() = default;
		explicit RenderBatch(GLuint offset_, GLuint numVertices_, GLuint texture_) :
			offset(offset_),
			numVertices(numVertices_),
			texture(texture_) {};
	};

	class SpriteBatch
	{
		GLuint m_vbo;
		GLuint m_vao;
		GlyphSortType m_sortType;

		std::vector<Glyph*> m_glyphPointers; // Reference glyphs for sorting
		std::vector<Glyph> m_glyphs; // Real used glyphs
		std::vector<RenderBatch> m_renderBatches;

		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);
	public:
		explicit SpriteBatch();
		explicit SpriteBatch(const SpriteBatch& sb);
		~SpriteBatch();

		void init();
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end();
		void pushBatch(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 & color);
		void pushBatch(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 & color, float angle);
		void pushBatch(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 & color, const glm::vec2 &dir);
		void renderBatches();
	};

}

