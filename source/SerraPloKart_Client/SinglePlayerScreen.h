#pragma once
#include <SerraPloEngine/IScreen.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/CarPhysics.h>
#include <SerraPloEngine/IAPhysics.h>
#include "RendererList.h"
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

#define MAX_POINT_LIGHTS 2
#define MAX_SPOT_LIGHTS 12
#define MAX_AI_ENEMIES 4

class AppClient;
class SinglePlayerScreen : public IScreen {
public:
	explicit SinglePlayerScreen() = default;
	~SinglePlayerScreen() = default;

	void Build(void) override;
	void Destroy(void) override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	void CheckInput(void);

	// Casted client main game pointer reference (IApp -> AppClient)
	AppClient *m_app;

	// Shader programs
	ShaderProgram m_mainProgram;
	ShaderProgram m_textProgram;
	ShaderProgram m_debugProgram;

	// Camera
	GLCamera m_camera;

	// Game objects
	RendererList m_renderer;
	GameObject m_player;
	GameObject m_aiEnemies[MAX_AI_ENEMIES];
	GameObject skybox;
	GameObject circuit;
	DebugObject debugCollisions;

	//Game physics
	CarPhysics m_carPhysics;
	std::vector<glm::vec2> m_aiPath;
	AIPhysics m_aiPhysics;

	// Lights
	DirLight m_dirLight;
	PointLight m_pointLights[MAX_POINT_LIGHTS];
	SpotLight m_spotLights[MAX_SPOT_LIGHTS];
	SpotLight m_carLights;
};
