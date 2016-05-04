#pragma once 

#include <string>
#include "Transform.h"
#include "GLMesh.h"
#include "GLTexture.h"

namespace SerraPlo {

	class GameObjectManager;
	class GameObject {
	public:
		std::string id{""};
		Transform transform;
		GLMesh mesh;
		GLTexture texture;
		explicit GameObject() = default;
		explicit GameObject(const std::string &fileId, const std::string &fileMesh, const std::string &fileTexture) : 
			id{ fileId }, 
			mesh{ fileMesh.c_str() }, 
			texture{ fileTexture.c_str() } {};
		virtual ~GameObject() = default;
	};

}