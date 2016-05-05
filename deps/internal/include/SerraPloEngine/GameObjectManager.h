#pragma once
#include <JsonBox/JsonBox.h>
#include <map>
#include "GameObject.h"
#include "ResourceManager.h"

#ifdef _DEBUG
#pragma comment(lib, "JsonBox_d.lib")
#else
#pragma comment(lib, "JsonBox.lib")
#endif

namespace SerraPlo {

	class GameObjectManager {
	public:
		std::map<std::string, GameObject> gameObjectList;
		explicit GameObjectManager() = default;
		explicit GameObjectManager(const std::string &filePath) {
			Load(filePath);
		};
		~GameObjectManager() {
			for (auto& entity : this->gameObjectList) {
				glDeleteVertexArrays(1, &entity.second.mesh.vao);
				glDeleteBuffers(1, &entity.second.mesh.vbo);
				glDeleteBuffers(1, &entity.second.mesh.ebo);
			}
		}
		void Load(const std::string &filePath) {
			JsonBox::Value fileData;
			fileData.loadFromFile(filePath);
			JsonBox::Object wrapper{ fileData.getObject() };
			for (auto entity : wrapper) {
				std::string key{ entity.first };
				JsonBox::Object properties{ entity.second.getObject() };
				//GameObject *tempGameObject = new() GameObject(entity.first, LoadAsset(properties["model"].getString()), LoadAsset(properties["texture"].getString()));
				this->gameObjectList[key] = GameObject(entity.first, LoadAsset(properties["model"].getString()), LoadAsset(properties["texture"].getString()));
				/// Load transform attributes
				JsonBox::Array tempArray = properties["position"].getArray();
				this->gameObjectList[key].transform.position = { tempArray[0].getFloat(), tempArray[1].getFloat(), tempArray[2].getFloat() };
				tempArray = properties["rotation"].getArray();
				this->gameObjectList[key].transform.rotation = { tempArray[0].getFloat(), tempArray[1].getFloat(), tempArray[2].getFloat() };
				tempArray = properties["scale"].getArray();
				this->gameObjectList[key].transform.scale = { tempArray[0].getFloat(), tempArray[1].getFloat(), tempArray[2].getFloat() };
			}
		}
	};

}
