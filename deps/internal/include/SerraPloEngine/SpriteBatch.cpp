#include "SpriteBatch.h"
#include <algorithm>

namespace SerraPlo {

	glm::vec2 Glyph::RotatePoint(glm::vec2 pos, float angle) const {
		return glm::vec2(pos.x*cos(angle) - pos.y*sin(angle), pos.x*sin(angle) + pos.y*cos(angle));
	}

	Glyph::Glyph(GLuint texture_, float depth_, const glm::vec4 & destRect, const glm::vec4 & uvRect, const ColorRGBA8 &color) :
		texture(texture_),
		depth(depth_) 
	{
		topLeft.position = { destRect.x, destRect.y + destRect.w };
		topLeft.uv = { uvRect.x, uvRect.y + uvRect.w };
		topLeft.color = color;
	
		bottomLeft.position = { destRect.x, destRect.y };
		bottomLeft.uv = { uvRect.x, uvRect.y };
		bottomLeft.color = color;
	
		bottomRight.position = { destRect.x + destRect.z, destRect.y };
		bottomRight.uv = { uvRect.x + uvRect.z, uvRect.y };
		bottomRight.color = color;

		topRight.position = { destRect.x + destRect.z, destRect.y + destRect.w };
		topRight.uv = { uvRect.x + uvRect.z, uvRect.y + uvRect.w };
		topRight.color = color;
	}

	Glyph::Glyph(GLuint texture_, float depth_, const glm::vec4 & destRect, const glm::vec4 & uvRect, const ColorRGBA8 & color, float angle) :
		texture(texture_),
		depth(depth_) 
	{
		const glm::vec2 halfDims{ destRect.z / 2.0f, destRect.w / 2.0f };
		//points centered at origin
		glm::vec2 tl{ -halfDims.x, halfDims.y };
		glm::vec2 bl{ -halfDims.x, -halfDims.y };
		glm::vec2 br{ halfDims.x, -halfDims.y };
		glm::vec2 tr{ halfDims.x, halfDims.y };
		//rotate points
		tl = RotatePoint(tl, angle) + halfDims;
		bl = RotatePoint(bl, angle) + halfDims;
		br = RotatePoint(br, angle) + halfDims;
		tr = RotatePoint(tr, angle) + halfDims;

		topLeft.position = { destRect.x + tl.x, destRect.y + tl.y };
		topLeft.uv = { uvRect.x, uvRect.y + uvRect.w };
		topLeft.color = color;

		bottomLeft.position = { destRect.x + bl.x, destRect.y + bl.y };
		bottomLeft.uv = { uvRect.x, uvRect.y };
		bottomLeft.color = color;

		bottomRight.position = { destRect.x + br.x, destRect.y + br.y };
		bottomRight.uv = { uvRect.x + uvRect.z, uvRect.y };
		bottomRight.color = color;

		topRight.position = { destRect.x + tr.x, destRect.y + tr.y };
		topRight.uv = { uvRect.x + uvRect.z, uvRect.y + uvRect.w };
		topRight.color = color;
	}
	
	SpriteBatch::SpriteBatch() : m_vbo(0), m_vao(0), m_sortType(GlyphSortType::NONE)
	{}

	SpriteBatch::SpriteBatch(const SpriteBatch& sb): m_vbo(sb.m_vbo), m_vao(sb.m_vao), m_sortType(sb.m_sortType)
	{
		m_glyphPointers.clear();
		for (auto gp : sb.m_glyphPointers) m_glyphPointers.push_back(new Glyph(*gp));
		m_glyphs.clear();
		for (auto g : sb.m_glyphs) m_glyphs.push_back(g);
		m_renderBatches.clear();
		for (auto rb : sb.m_renderBatches) m_renderBatches.push_back(rb);
	}

	SpriteBatch::~SpriteBatch() {
		for (auto g : m_glyphPointers) if(g) delete g, g = nullptr;
	}

	void SpriteBatch::init() {
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType){
		m_sortType = sortType;
		m_renderBatches.clear();
		m_glyphs.clear();
	}

	void SpriteBatch::end(){
		m_glyphPointers.resize(m_glyphs.size());
		for (unsigned i = 0; i < m_glyphs.size(); i++) m_glyphPointers[i] = &m_glyphs[i];
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::pushBatch(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 &color) {
		m_glyphs.emplace_back(texture, depth, destRect, uvRect, color);
	}

	void SpriteBatch::pushBatch(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 & color, float angle) {
		m_glyphs.emplace_back(texture, depth, destRect, uvRect, color, angle);
	}

	void SpriteBatch::pushBatch(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 & color, const glm::vec2 &dir) {
		const glm::vec2 right(1.0f, 0.0f);
		auto angle = acos(glm::dot(dir, right));
		if (dir.y < 0.0f) angle = -angle;
		m_glyphs.emplace_back(texture, depth, destRect, uvRect, color, angle);
	}

	void SpriteBatch::renderBatches() {
		glBindVertexArray(m_vao);
		for (const auto &rb : m_renderBatches) {
			glBindTexture(GL_TEXTURE_2D, rb.texture);
			glDrawArrays(GL_TRIANGLES, rb.offset, rb.numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches() {
		if (m_glyphPointers.empty()) return;

		std::vector<Vertex> vertices;
		vertices.resize(m_glyphPointers.size() * 6); //6 vertices for each glyph

		auto offset = 0; //current offset
		auto curVertex = 0; //current vertex
		//Add the first batch
		m_renderBatches.emplace_back(offset, 6, m_glyphPointers[0]->texture); //push_back that creates an object automatically with the parameters
		vertices[curVertex++] = m_glyphPointers[0]->topLeft;
		vertices[curVertex++] = m_glyphPointers[0]->bottomLeft;
		vertices[curVertex++] = m_glyphPointers[0]->bottomRight;
		vertices[curVertex++] = m_glyphPointers[0]->bottomRight;
		vertices[curVertex++] = m_glyphPointers[0]->topRight;
		vertices[curVertex++] = m_glyphPointers[0]->topLeft;
		offset += 6;
		for (unsigned curGlyph = 1; curGlyph < m_glyphPointers.size(); curGlyph++) {
			if (m_glyphPointers[curGlyph]->texture != m_glyphPointers[curGlyph - 1]->texture) {
				m_renderBatches.emplace_back(offset, 6, m_glyphPointers[curGlyph]->texture);
			}
			else m_renderBatches.back().numVertices += 6;
			vertices[curVertex++] = m_glyphPointers[curGlyph]->topLeft;
			vertices[curVertex++] = m_glyphPointers[curGlyph]->bottomLeft;
			vertices[curVertex++] = m_glyphPointers[curGlyph]->bottomRight;
			vertices[curVertex++] = m_glyphPointers[curGlyph]->bottomRight;
			vertices[curVertex++] = m_glyphPointers[curGlyph]->topRight;
			vertices[curVertex++] = m_glyphPointers[curGlyph]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(Vertex), vertices.data());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray() {
		if (!m_vao) glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		if (!m_vbo) glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		//use attribute arrays
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
		//color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
		//uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs() {
		switch (m_sortType)
		{
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareFrontToBack);
			break;
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareBackToFront);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareTexture);
			break;
		default:
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph * a, Glyph * b) {
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareBackToFront(Glyph * a, Glyph * b) {
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph * a, Glyph * b) {
		return (a->texture < b->texture);
	}

}