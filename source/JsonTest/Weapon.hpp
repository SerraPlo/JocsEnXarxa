#pragma once
#include <string>
#include "Item.hpp"

class EntityManager;
class Weapon : public Item {
public:
	int damage;

	Weapon(const std::string &id, const std::string &name, const std::string &description, int damage);
	Weapon(const std::string &id, JsonBox::Value& v, EntityManager* mgr);

	// Load the armor from the Json value
	void load(JsonBox::Value& v, EntityManager* mgr) override;
};