#pragma once
#include <SerraPloEngine/IScreen.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/CarPhysics.h>
#include <SerraPloEngine/GLText.h>
#include <SerraPloEngine/AIPhysics.h>
#include "RendererList.h"
#include <vector>
#include <map>
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

#define MAX_POINT_LIGHTS 2
#define MAX_SPOT_LIGHTS 12
#define MAX_ENEMIES 9

class AppClient;
class MultiplayerScreen : public IScreen {
public:
	explicit MultiplayerScreen() = default;
	~MultiplayerScreen() = default;

	void Build(void) override;
	void Destroy(void) override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	void CheckInput(void);
	void ProcessMsgs(void);
	void UpdateEnemies(void);
	void DoPhysics(void);

	// Casted client main game pointer reference (IApp -> AppClient)
	AppClient *m_app;

	// Shader programs
	ShaderProgram m_mainProgram;
	ShaderProgram m_textProgram;
	ShaderProgram m_debugProgram;

	// Camera
	GLCamera m_camera;

	// GameObjects renderer list
	RendererList m_renderer;

	//Player
	struct {
		GameObject body;
		GameObject wheels[4];
		GLText nickIdentifier;
	} m_player;

	// Enemies
	struct Enemy {
		Transform targetTransform;
		GameObject body;
		GameObject wheels[4];
		GLText nickIdentifier;
	}; std::map<std::string, Enemy> m_enemies;

	// Atrezzo
	GameObject skybox;
	GameObject circuit;
	DebugObject debugCollisions;

	GameObject debugIA1;
	GameObject debugIA2;

	//Game physics
	CarPhysics m_carPhysics;
	std::vector<glm::vec2> m_aiPath;
	AIPhysics m_aiPhysics;

	// Lights
	DirLight m_dirLight;
	PointLight m_pointLights[MAX_POINT_LIGHTS];
	SpotLight m_spotLights[MAX_SPOT_LIGHTS];
	SpotLight m_carLights;

	Transform myServTrans;
};
