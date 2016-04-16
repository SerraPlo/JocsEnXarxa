#pragma once
#include <string>
#include "Item.hpp"

class EntityManager;
class Armor : public Item {
public:
	int defense;

	Armor(const std::string &id, const std::string &name, const std::string &description, int defense);
	Armor(const std::string &id, JsonBox::Value& v, EntityManager* mgr);

	// Load the armor from the Json value
	void Load(JsonBox::Value& v, EntityManager* mgr) override;
};

