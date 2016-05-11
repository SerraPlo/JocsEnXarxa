#pragma once 

#include <string>
#include "Transform.h"
#include "GLMesh.h"

namespace SerraPlo {

	class GameObject {
	public:
		std::string id{""};
		Transform transform;
		GLModel model;
		explicit GameObject() = default;
		explicit GameObject(const std::string &fileId, const char* meshPath, const char* diffusePath,  const char* normalPath, 
							JsonBox::Array specular, JsonBox::Array emissive, float shininess) :
			id{ fileId },
			model(meshPath, diffusePath, normalPath, specular, emissive, shininess) {}
			//material(diffusePath, normalPath, specular, emissive, shininess) {};
		virtual ~GameObject() = default;
	};

}