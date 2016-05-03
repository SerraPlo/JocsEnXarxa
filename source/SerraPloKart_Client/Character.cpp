#include "Character.h"
#include <SerraPloEngine/EntityManager.h>

Character::Character(const std::string & id, const std::string & name, const std::string & description) {
	this->name = name;
	this->description = description;
}

Character::Character(const std::string & id, JsonBox::Value & v, EntityManager * mgr) {
	this->Character::Load(v, mgr);
}

void Character::Load(JsonBox::Value & v, EntityManager * mgr) {
	JsonBox::Object o = v.getObject();
	this->name = o["name"].getString();
	this->description = o["description"].getString();
	testMesh.Load(ResourceManager::LoadAsset("models/seahorse/seahorse.obj").c_str());
}

// Specialisation
template <> std::string entityToString<Character>() { return "character"; }

// Instantiation
template void EntityManager::LoadJson<Character>(std::string);
template Character* EntityManager::GetEntity<Character>(std::string);