#pragma once
#include <JsonBox/JsonBox.h>
#include "GLTexture.h"
#include <glm/glm.hpp>

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
		glm::vec3 emissive;
		float shininess;
		explicit GLMaterial() = default;
		explicit GLMaterial(const char* diffusePath, const char* normalPath, JsonBox::Array specularPath, JsonBox::Array emissiveFile, float shininessFile) :
			diffuse(diffusePath),
			normal(normalPath),
			specular(specularPath[0].getFloat(), specularPath[1].getFloat(), specularPath[2].getFloat()),
			emissive(emissiveFile[0].getFloat(), emissiveFile[1].getFloat(), emissiveFile[2].getFloat()),
			shininess{ shininessFile } {};
	};

}
