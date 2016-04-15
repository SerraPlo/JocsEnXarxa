#include "Weapon.hpp"
#include "Item.hpp"
#include "EntityManager.hpp"

Weapon::Weapon(const std::string &id, const std::string &name, const std::string &description, int damage) : Item(id, name, description) {
	this->damage = damage;
}

Weapon::Weapon(const std::string &id, JsonBox::Value& v, EntityManager* mgr) : Item(id, v, mgr) {
	this->Weapon::load(v, mgr);
}

void Weapon::load(JsonBox::Value& v, EntityManager* mgr) {
	JsonBox::Object o = v.getObject();
	this->damage = o["damage"].getInteger();
}

// Specialisation
template <> std::string entityToString<Weapon>() { return "weapon"; }

// Instantiation
template void EntityManager::loadJson<Weapon>(std::string);
template Weapon* EntityManager::getEntity<Weapon>(std::string);