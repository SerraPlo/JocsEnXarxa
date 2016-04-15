#pragma once
#include <JsonBox/JsonBox.h>
#include <string>

#ifdef _DEBUG
#pragma comment(lib, "JsonBox_d.lib")
#else
#pragma comment(lib, "JsonBox.lib")
#endif

class EntityManager;
class Entity {
public:
	std::string id;
	explicit Entity(const std::string &id) { this->id = id; }
	virtual ~Entity() {} //enable polimorphism
	virtual void load(JsonBox::Value& v, EntityManager* mgr) = 0; //enable pure virtual class (disable instantiation)
};