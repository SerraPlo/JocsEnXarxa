#include "Armor.hpp"
#include "Item.hpp"
#include "EntityManager.hpp"

Armor::Armor(const std::string &id, const std::string &name, const std::string &description, int defense) : Item(id, name, description) {
	this->defense = defense;
}

Armor::Armor(const std::string &id, JsonBox::Value& v, EntityManager* mgr) : Item(id, v, mgr) {
	this->Armor::Load(v, mgr);
}

void Armor::Load(JsonBox::Value& v, EntityManager* mgr) {
	JsonBox::Object o = v.getObject();
	this->defense = o["defense"].getInteger();
}

template <> std::string entityToString<Armor>() { return "armor"; }

template void EntityManager::LoadJson<Armor>(std::string);
template Armor* EntityManager::GetEntity<Armor>(std::string);
