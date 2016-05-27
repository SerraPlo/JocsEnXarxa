#pragma once
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/GLLight.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SerraPloEngine/GLCamera.h>
#include <SerraPloEngine/AssetManager.h>
#include <vector>
using namespace SerraPlo;

class RendererList {
	std::vector<GameObject*> m_objectList;
	std::vector<DebugObject> m_debugList;
	DirLight *m_dirLight;
	std::vector<PointLight*> m_pointLightList;
	std::vector<SpotLight*> m_spotLightList;
public:
	static bool DEBUG_DRAW;
	static GLuint DEBUG_MODE;

	explicit RendererList() = default;
	~RendererList() = default;

	void Add(GameObject *newObject);
	void Add(DebugObject newObject);
	void Add(DirLight *newLight);
	void Add(PointLight *newLight);
	void Add(SpotLight *newLight);

	void SendLightAttributes(ShaderProgram &program, GLCamera &camera);
	static void SendMaterialAttributes(ShaderProgram &program, GLCamera &camera);

	void DrawObjects(ShaderProgram &program, GLCamera &camera);
	void DrawDebug(ShaderProgram &program, GLCamera &camera);

	void Clear();
};

