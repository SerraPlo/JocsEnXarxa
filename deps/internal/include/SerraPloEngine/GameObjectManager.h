#pragma once
#include <map>
#include "GameObject.h"
#include "ResourceManager.h"

namespace SerraPlo {
	class GameObjectManager {
		std::map<std::string, GameObject> gameObjectList;
	public:
		explicit GameObjectManager() = default;
		explicit GameObjectManager(const std::string &filePath) {
			Load(filePath);
		};
		~GameObjectManager() {
			//for (auto& entity : this->gameObjectList) {
				//glDeleteVertexArrays(1, &entity.second.model.meshData->vao);
				//glDeleteBuffers(1, &entity.second.mesh.vbo);
				//glDeleteBuffers(1, &entity.second.mesh.ebo);
				//delete entity.second;
			//}
		}
		bool Empty() {
			return gameObjectList.empty();
		}
		void Load(const std::string &filePath) {
			JsonBox::Value fileData;
			fileData.loadFromFile(filePath);
			JsonBox::Object wrapper{ fileData.getObject() };
			for (auto entity : wrapper) {
				std::string key{ entity.first };
				JsonBox::Object properties{ entity.second.getObject() };
				this->gameObjectList[key] = GameObject(entity.first, 
													   LoadAsset(properties["model"].getString()).c_str(), 
													   !properties["diffuse"].isNull() ? LoadAsset(properties["diffuse"].getString()).c_str() : nullptr,
													   !properties["normal"].isNull() ? LoadAsset(properties["normal"].getString()).c_str() : nullptr,
													   properties["specular"].getArray(),
													   !properties["emissive"].isNull() ? properties["emissive"].getArray() : JsonBox::Array{0, 0, 0},
													   properties["shininess"].getFloat());
				/// Load transform attributes
				JsonBox::Array tempArray = properties["position"].getArray();
				this->gameObjectList[key].transform.position = { tempArray[0].getFloat(), tempArray[1].getFloat(), tempArray[2].getFloat() };
				tempArray = properties["rotation"].getArray();
				this->gameObjectList[key].transform.rotation = { tempArray[0].getFloat(), tempArray[1].getFloat(), tempArray[2].getFloat() };
				tempArray = properties["scale"].getArray();
				this->gameObjectList[key].transform.scale = { tempArray[0].getFloat(), tempArray[1].getFloat(), tempArray[2].getFloat() };
			}
			std::cout << "Assets loaded" << std::endl;
		}
		GameObject& Find(const std::string &str) {
			auto it = gameObjectList.find(str);
			if (it == gameObjectList.end()) SP_THROW_ERROR("Could not find game object [" + str + "] inside manager list"); 
			return it->second;
		}
	};

}
