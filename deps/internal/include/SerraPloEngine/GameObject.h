#pragma once 
#include <JsonBox/JsonBox.h>
#include <string>
#include "Transform.h"
#include "GLMesh.h"
#include "GLTexture.h"

#ifdef _DEBUG
#pragma comment(lib, "JsonBox_d.lib")
#else
#pragma comment(lib, "JsonBox.lib")
#endif

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