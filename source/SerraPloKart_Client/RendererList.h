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
	DirLight *m_dirLight;
	std::vector<PointLight*> m_pointLightList;
	std::vector<SpotLight*> m_spotLightList;
public:
	explicit RendererList() = default;
	~RendererList() = default;
	void Add(GameObject *newObject);
	void Add(DirLight *newLight);
	void Add(PointLight *newLight);
	void Add(SpotLight *newLight);
	void DrawObjects(ShaderProgram &program, Camera &camera);
	void DrawLights(ShaderProgram &program, Camera &camera);
};

