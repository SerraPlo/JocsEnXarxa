#pragma once
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/Light.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SerraPloEngine/Camera.h>
#include <vector>
using namespace SerraPlo;

#define LIGHT_DEBUG_MODE true

class RendererList {
	std::vector<GameObject*> m_objectList;
	std::vector<BaseLight> m_lightList;
public:
	explicit RendererList() = default;
	~RendererList() = default;
	void Add(GameObject *newObject);
	void Add(BaseLight newObject);
	void DrawObjects(ShaderProgram &program, Camera &camera);
	void DrawLights(ShaderProgram &program, Camera &camera);
};

