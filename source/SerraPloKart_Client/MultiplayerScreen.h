#pragma once
#include <SerraPloEngine/IScreen.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/CarPhysics.h>
#include <SerraPloEngine/GLText.h>
#include "RendererList.h"
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

#define MAX_POINT_LIGHTS 2
#define MAX_SPOT_LIGHTS 12
#define MAX_ENEMIES 9
#define MAX_POINTS_PATH 12

struct SimplePath {
	glm::vec2 pathArray[MAX_POINTS_PATH];
	int pathOccupation = 0;
};

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

	void UpdateEnemies(float dt);
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
	GameObject m_playerwheels[4];
	GameObject m_enemies[MAX_ENEMIES];
	GameObject m_enemyWheels[MAX_ENEMIES][4];
	GameObject skybox;
	GameObject circuit;
	DebugObject debugCollisions;
	GameObject debugIA;

	// Text objects
	GLText m_textNick; 
	GLText m_textNickEnemies[MAX_ENEMIES];

	//Game physics
	CarPhysics m_carPhy;

	// Lights
	DirLight m_dirLight;
	PointLight m_pointLights[MAX_POINT_LIGHTS];
	SpotLight m_spotLights[MAX_SPOT_LIGHTS];
	SpotLight m_carLights;

	//IA
	SimplePath simplePath;
	int currentSegment = 0;
	int pathDirection = 1;
	float K_SIMPLE_PATH_ARRIVAL_DISTANCE = 10.0f;
	glm::vec2 desiredVelocity = {};
	glm::vec2 steeringForce = {};
	glm::vec2 acceleration = {};
	float mass = 1.0f;
	glm::vec2 speedIA = {};
	glm::vec2 speedCounterIA = {};
	glm::vec2 realSpeedIA = {};
	// Constants
	float K_MAX_SPEED = 15.0f;
	//float K_MAX_STEER_FORCE = 6.0f * 60.0f; // For Seek + Flocking
	float K_MAX_STEER_FORCE = 15.0f * 60.0f; // Normal
};
