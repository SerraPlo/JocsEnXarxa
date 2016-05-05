#pragma once
#include "GLTexture.h"

namespace SerraPlo {

	struct GLMaterial {
		GLTexture diffuse;
		GLTexture normal;
		GLTexture specular;
		float shininess;
		explicit GLMaterial() = default;
		explicit GLMaterial(const char* diffusePath, const char* normalPath, const char* specularPath, float shininessFile) :
			diffuse(diffusePath),
			normal(normalPath),
			specular(specularPath),
			shininess{ shininessFile } {};
	};

}
