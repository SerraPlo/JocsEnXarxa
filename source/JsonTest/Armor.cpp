#include "Armor.hpp"
#include "Item.hpp"
#include "EntityManager.hpp"

Armor::Armor(const std::string &id, const std::string &name, const std::string &description, int defense) : Item(id, name, description) {
	this->defense = defense;
}

Armor::Armor(const std::string &id, JsonBox::Value& v, EntityManager* mgr) : Item(id, v, mgr) {
	this->Armor::load(v, mgr);
}

void Armor::load(JsonBox::Value& v, EntityManager* mgr) {
	JsonBox::Object o = v.getObject();
	this->defense = o["defense"].getInteger();
}

template <> std::string entityToString<Armor>() { return "weapon"; }

template void EntityManager::loadJson<Armor>(std::string);
template Armor* EntityManager::getEntity<Armor>(std::string);
