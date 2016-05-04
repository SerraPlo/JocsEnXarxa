#pragma once
#include <map>
#include "GameObject.h"
#include "ResourceManager.h"

namespace SerraPlo {

	class GameObjectManager {
	public:
		std::map<std::string, GameObject*> gameObjectList;
		explicit GameObjectManager() = default;
		explicit GameObjectManager(const std::string &filePath) {
			Load(filePath);
		};
		~GameObjectManager() {
			for (auto& entity : this->gameObjectList) delete entity.second;
		}
		void Load(const std::string &filePath) {
			JsonBox::Value fileData;
			fileData.loadFromFile(filePath);
			JsonBox::Object wrapper{ fileData.getObject() };
			for (auto entity : wrapper) {
				std::string key{ entity.first };
				JsonBox::Object properties{ entity.second.getObject() };
				GameObject *tempGameObject = new GameObject(entity.first);
				/// Load transform attributes
				Transform *tempTransform = new Transform;
				JsonBox::Array tempArray = properties["position"].getArray();
				tempTransform->position = { tempArray[0].getFloat(), tempArray[0].getFloat(), tempArray[0].getFloat() };
				tempArray = properties["rotation"].getArray();
				tempTransform->rotation = { tempArray[0].getFloat(), tempArray[0].getFloat(), tempArray[0].getFloat() };
				tempArray = properties["scale"].getArray();
				tempTransform->scale = { tempArray[0].getFloat(), tempArray[0].getFloat(), tempArray[0].getFloat() };
				tempGameObject->transform = tempTransform;
				/// Load mesh
				tempGameObject->mesh = new GLMesh(LoadAsset(properties["model"].getString()).c_str());
				/// Load texture
				tempGameObject->texture = new GLTexture(LoadAsset(properties["texture"].getString()).c_str());
				this->gameObjectList[key] = tempGameObject;
			}
		}
	};

}
