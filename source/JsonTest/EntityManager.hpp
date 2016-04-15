#pragma once
#include <string>
#include <map>
#include "Entity.hpp"

class EntityManager {
	std::map<std::string, Entity*> data;
public:
	EntityManager() = default;
	~EntityManager() {
		for (auto& entity : this->data) delete entity.second; // Deallocate reserved memory for data
	}

	template<typename T>
	void loadJson(std::string filename) { // Load JSON file & determine how to save data according to datatype T
		JsonBox::Value fileData;
		fileData.loadFromFile(filename);

		JsonBox::Object wrapper { fileData.getObject() };
		for (auto entity : wrapper) {
			std::string key { entity.first };
			this->data[key] = dynamic_cast<Entity*>(new T(key, entity.second, this)); // Cast new T object to Entity base pointer object
		}
	}

	template<typename T>
	T* getEntity(std::string id) { // Return entity given an id
		if (id.substr(0, entityToString<T>().size()) == entityToString<T>()) // Check if id contains string matching to datatype T
			return dynamic_cast<T*>(this->data.at(id)); // Return entity with key id as datatype T
		return nullptr;
	}
};

template <typename T>
std::string entityToString(); // Convert derived entity type to its id prefix (e.g. Item -> "item")