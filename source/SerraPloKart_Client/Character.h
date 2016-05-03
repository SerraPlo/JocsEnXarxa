#pragma once
#include <SerraPloEngine/Entity.h>
#include <string>
#include <GL/glew.h>

class Character : public Entity {
public:
	std::string name;
	std::string description;
	GLTexture* texture;

	Character(const std::string &id, const std::string &name, const std::string &description);
	Character(const std::string &id, JsonBox::Value& v, EntityManager* mgr);

	virtual void Load(JsonBox::Value& v, EntityManager* mgr) override; // Load item information from JSON value
};

