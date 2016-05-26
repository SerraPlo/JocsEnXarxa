#pragma once
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/Light.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SerraPloEngine/Camera.h>
#include <SerraPloEngine/GameObjectManager.h>
#include <vector>
using namespace SerraPlo;

class RendererList {
	std::vector<GameObject*> m_objectList;
	std::vector<GameObject*> m_debugList;
	DirLight *m_dirLight;
	std::vector<PointLight*> m_pointLightList;
	std::vector<SpotLight*> m_spotLightList;
public:
	static bool DEBUG_DRAW;
	static GLuint DEBUG_MODE;

	explicit RendererList() = default;
	~RendererList() = default;

	void Add(GameObject *newObject);
	void Add(GlobalGameObject *newObject);
	void AddDebug(GameObject *newObject);
	void Add(DirLight *newLight);
	void Add(PointLight *newLight);
	void Add(SpotLight *newLight);

	void SendLightAttributes(ShaderProgram &program, Camera &camera);
	static void SendMaterialAttributes(ShaderProgram &program, Camera &camera);

	void DrawObjects(ShaderProgram &program, Camera &camera, GameObjectManager &objectManager);
	void DrawDebug(ShaderProgram &program, Camera &camera);

	void Clear();
};

