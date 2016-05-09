#pragma once
#include <JsonBox/JsonBox.h>
#include "GLTexture.h"

#ifdef _DEBUG
#pragma comment(lib, "JsonBox_d.lib")
#else
#pragma comment(lib, "JsonBox.lib")
#endif

namespace SerraPlo {

	struct GLMaterial {
		GLTexture diffuse;
		GLTexture normal;
		glm::vec3 specular;
		float shininess;
		explicit GLMaterial() = default;
		explicit GLMaterial(const char* diffusePath, const char* normalPath, JsonBox::Array specularPath, float shininessFile) :
			diffuse(diffusePath),
			normal(normalPath),
			specular(specularPath[0].getFloat(), specularPath[1].getFloat(), specularPath[2].getFloat()),
			shininess{ shininessFile } {};
	};

}
