#pragma once
#include <map>
#include "Entity.hpp"

template <typename T>
// ReSharper disable once CppFunctionIsNotImplemented
std::string entityToString(); // Convert derived entity type to its id prefix (e.g. Item -> "item")

class EntityManager {
	std::map<std::string, Entity*> dataMap;
	//std::vector<Entity*> dataList;
public:
	EntityManager() = default;
	~EntityManager() {
		for (auto& entity : this->dataMap) delete entity.second; // Deallocate reserved memory for data
	}

	template<typename T>
	void LoadJson(std::string filename) { // Load JSON file & determine how to save data according to datatype T
		JsonBox::Value fileData;
		fileData.loadFromFile(filename);

		JsonBox::Object wrapper { fileData.getObject() };
		for (auto entity : wrapper) {
			std::string key { entity.first };
			this->dataMap[key] = dynamic_cast<Entity*>(new T(key, entity.second, this)); // Cast new T object to Entity base pointer object
			//this->dataList.push_back(this->dataMap[key]);
		}
	}

	template<typename T>
	T* GetEntity(std::string id) { // Return entity given an id
		if (id.substr(0, entityToString<T>().size()) == entityToString<T>()) // Check if id contains string matching to datatype T
			return dynamic_cast<T*>(this->dataMap.at(id)); // Return entity with key id as datatype T
		return nullptr;
	}

	template<typename T>
	T* GetRandEntity() {
		std::vector<Entity*> tElements;
		auto entityString = entityToString<T>();
		for (auto entity : dataMap)
			if (entity.second->id.substr(0, entityString.size()) == entityString)
				tElements.push_back(entity.second);
		if (!tElements.empty()) return dynamic_cast<T*>(tElements[rand() % tElements.size()]);
		return nullptr;
	}
};