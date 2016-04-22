#pragma once
#include "Vertex.h"
#include "SpriteBatch.h"
#include <glm/detail/type_vec2.hpp>

namespace SerraPlo {

class Light {
public:
	explicit Light() = default;
	~Light() = default;

	void draw(SerraPlo::SpriteBatch &spriteBatch) {
		glm::vec4 destRect(position.x - size*0.5f, position.y - size*0.5f, size, size);
		spriteBatch.pushBatch(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color, 0.0f);
	};

	SerraPlo::ColorRGBA8 color;
	glm::vec2 position;
	float size;
};

}