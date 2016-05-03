#include "Renderer3D.h"

namespace SerraPlo {

	Renderer3D::Renderer3D() {
		
	}

	Renderer3D::~Renderer3D() {
		for (auto mesh : meshList) if (mesh) delete mesh;
	}

	void Renderer3D::LoadMeshes(const std::string &filePath) {
		JsonBox::Value fileData;
		fileData.loadFromFile(filePath);

		JsonBox::Object wrapper{ fileData.getObject() };
		for (auto entity : wrapper) {
			std::string key{ entity.first };
			this->dataMap[key] = dynamic_cast<Entity*>(new T(key, entity.second, this)); // Cast new T object to Entity base pointer object
		}
	}

}
