#pragma once
#include "Entity.hpp"

class EntityManager;
class Item : public Entity {
public:
	std::string name;
	std::string description;

	Item(const std::string &id, const std::string &name, const std::string &description);
	Item(const std::string &id, JsonBox::Value& v, EntityManager* mgr);

	virtual void Load(JsonBox::Value& v, EntityManager* mgr) override; // Load item information from JSON value
};