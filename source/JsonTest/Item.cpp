#include "Item.hpp"
#include "EntityManager.hpp"

Item::Item(const std::string &id, const std::string &name, const std::string &description) : Entity(id) {
	this->name = name;
	this->description = description;
}

Item::Item(const std::string &id, JsonBox::Value& v, EntityManager* mgr) : Entity(id) {
	this->Item::Load(v, mgr);
}

void Item::Load(JsonBox::Value& v, EntityManager* mgr) {
	JsonBox::Object o = v.getObject();
	this->name = o["name"].getString();
	this->description = o["description"].getString();
}

// Specialisation
template <> std::string entityToString<Item>() { return "item"; }

// Instantiation
template void EntityManager::LoadJson<Item>(std::string);
template Item* EntityManager::GetEntity<Item>(std::string);

