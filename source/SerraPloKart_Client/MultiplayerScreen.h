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
struct input10 {
	bool w[10];
	bool a[10];
	bool s[10];
	bool d[10];
	float dt[10];
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
	GameObject *m_player;
	GameObject m_playerwheels[4];
	GameObject markersCol[116];
	CarPhysics m_carPhy;
	// Lights
	DirLight m_dirLight;
	PointLight m_pointLights[MAX_POINT_LIGHTS];
	SpotLight m_spotLights[MAX_SPOT_LIGHTS];
	// Nick texture set above the player
	GLText m_textNick;

	int m_inputCounter = 0;
	input10 m_in2send;
};
