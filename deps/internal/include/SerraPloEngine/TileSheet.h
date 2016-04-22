#pragma once
#include <GL\glew.h>
#include <glm/glm.hpp>

namespace SerraPlo {

class TileSheet {
public:
	explicit TileSheet() = default;
	~TileSheet() = default;

	void init(GLuint textureID, const glm::ivec2 &tileDims) {
		this->textureID = textureID;
		this->dims = tileDims;
	};

	glm::vec4 getUVs(int index) const {
		auto xTile = index % dims.x;
		auto yTile = index / dims.x;

		return glm::vec4 (xTile / static_cast<float>(dims.x), 
						  yTile / static_cast<float>(dims.y), 
						  1.0f / dims.x, 
						  1.0f / dims.y);
	};

	GLuint textureID;
	glm::ivec2 dims;
};

}
