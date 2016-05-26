#pragma once
#include <SerraPloEngine/IScreen.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SerraPloEngine/Camera.h>
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/CarPhysics.h>
#include "RendererList.h"
#include <SerraPloEngine/Primitives.h>
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

#define MAX_POINT_LIGHTS 1
#define MAX_SPOT_LIGHTS 1
#define MAX_PLAYERS 10

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

	void UpdateEnemies();
private:
	void CheckInput(void);

	// Casted client main game pointer reference (IApp -> AppClient)
	AppClient *m_app;
	// Shader programs
	ShaderProgram m_mainProgram;
	ShaderProgram m_lightProgram;
	// Camera
	Camera m_camera;
	// Game objects
	RendererList m_renderer;

	GameObject m_player;
	GameObject m_playerwheels[4];

	GameObject m_enemies[MAX_PLAYERS];
	GameObject m_enemyWheels[MAX_PLAYERS][4];

	GameObject circuit;
	GameObject skybox;
	GameObject debugCollision;

	GLText m_textNick; 
	GLText m_textNickEnemies[MAX_PLAYERS];

	CarPhysics m_carPhy;
	// Lights
	DirLight m_dirLight;
	PointLight m_pointLights[MAX_POINT_LIGHTS];
	SpotLight m_spotLights[MAX_SPOT_LIGHTS];

};
