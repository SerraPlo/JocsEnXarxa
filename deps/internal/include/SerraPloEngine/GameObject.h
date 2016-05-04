#pragma once 

#include <string>
#include "Transform.h"
#include "GLMesh.h"
#include "GLTexture.h"

namespace SerraPlo {

	class GameObjectManager;
	class GameObject {
	public:
		std::string id			{""};
		Transform *transform	{ nullptr };
		GLMesh *mesh			{ nullptr };
		GLTexture *texture		{ nullptr };
		explicit GameObject(const std::string &id) { this->id = id; }
		virtual ~GameObject() { delete transform, delete mesh, delete texture; };
	};

}