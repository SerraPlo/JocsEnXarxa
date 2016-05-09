#pragma once 

#include <string>
#include "Transform.h"
#include "GLMesh.h"
#include "GLMaterial.h"

namespace SerraPlo {

	class GameObject {
	public:
		std::string id{""};
		Transform transform;
		GLMesh mesh;
		GLMaterial material;
		explicit GameObject() = default;
		explicit GameObject(const std::string &fileId, const char* meshPath, 
							const char* diffusePath,  const char* normalPath, JsonBox::Array specularFile, float shininessFile) :
			id{ fileId },
			mesh(meshPath),
			material(diffusePath, normalPath, specularFile, shininessFile) {};
		virtual ~GameObject() = default;
	};

}