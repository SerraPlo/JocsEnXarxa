#pragma once
#include "GLTexture.h"
#include <glm/glm.hpp>

namespace SerraPlo {
	struct GLMaterial {
		GLTexture diffuse;
		GLTexture normal;
		glm::vec3 specular;
		glm::vec3 emissive;
		float shininess;
	};
}
