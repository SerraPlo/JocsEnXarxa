#pragma once
#include <SerraPloEngine/IScreen.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/CarPhysics.h>
#include <SerraPloEngine/AIPhysics.h>
#include <SerraPloEngine/PowerUp.h>
#include "RendererList.h"
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

#define MAX_AI_ENEMIES 4
#define MAX_POINT_LIGHTS 2
#define MAX_STATIC_SPOT_LIGHTS 12
#define MAX_DYNAMIC_SPOT_LIGHTS MAX_AI_ENEMIES + 1

#define POWERUP_SPAWN_DELAY 5000
#define POWERUP_DETECT_DISTANCE 5.0f

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
	ShaderProgram m_screenProgram;
	ShaderProgram m_debugProgram;

	// Camera
	GLCamera m_camera;
	GLCamera m_minimapCamera;

	// GameObjects renderer list
	RendererList m_renderer;

	// Player
	struct {
		GameObject body;
		GameObject wheels[4];
		SpotLight light;
		PowerUp *powerUp;
	} m_player;

	// AI Enemies
	struct {
		GameObject body;
		GameObject wheels[4];
		SpotLight light;
		PowerUp *powerUp;
	} m_aiEnemies[MAX_AI_ENEMIES];

	// Atrezzo
	GameObject skybox;
	GameObject circuit;
	DebugObject debugCollisions;

	// Item Box & Power Ups
	struct : GameObject { clock_t activeCounter{ 0 }; } itemBox;
	std::vector<PowerUp*> powerUpList;

	//Game physics
	CarPhysics m_carPhysics;
	std::vector<glm::vec2> m_aiPath;
	AIPhysics m_aiPhysics;

	// Lights
	DirLight m_dirLight;
	PointLight m_pointLights[MAX_POINT_LIGHTS];
	SpotLight m_spotLights[MAX_STATIC_SPOT_LIGHTS];
};
