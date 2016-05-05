#pragma once
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <vector>

class SceneObjectList {
	std::vector<SerraPlo::GameObject*> m_objectList;
public:
	explicit SceneObjectList() = default;
	~SceneObjectList() = default;
	void Add(SerraPlo::GameObject *newObject);
	void Draw(SerraPlo::ShaderProgram &program);
};

