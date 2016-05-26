#pragma once 

#include <string>
#include "Transform.h"
#include "GLMesh.h"

namespace SerraPlo {

	class GameObject {
	public:
		std::string id{""};
		Transform transform;
		GLMaterial *materials{nullptr};
		int numMaterials{ 0 };
		explicit GameObject() = default;
		explicit GameObject(const std::string &fileId) : id{ fileId } {};
		virtual ~GameObject() = default;
		void Destroy() const { numMaterials > 1 ? delete[] materials : delete materials; };
	};

	class GlobalGameObject : public GameObject {
	public:
		GLModel model;
		explicit GlobalGameObject() = default;
		explicit GlobalGameObject(const std::string &fileId, const char* meshPath, const char* diffusePath, const char* normalPath,
							JsonBox::Array specular, JsonBox::Array emissive, float shininess) :
			GameObject(fileId),
			model(meshPath, diffusePath, normalPath, specular, emissive, shininess) {}
		virtual ~GlobalGameObject() = default;
	};

}