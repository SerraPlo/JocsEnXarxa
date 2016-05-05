#pragma once
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/Light.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SerraPloEngine/Camera.h>
#include <vector>

class RendererList {
	std::vector<SerraPlo::GameObject*> m_objectList;
	std::vector<SerraPlo::BaseLight*> m_lightList;
public:
	explicit RendererList() = default;
	~RendererList() = default;
	void Add(SerraPlo::GameObject *newObject);
	void Add(SerraPlo::BaseLight *newObject);
	void DrawObjects(SerraPlo::ShaderProgram &program, Camera &camera);
	void DrawLights(SerraPlo::ShaderProgram &program, Camera &camera);
};

