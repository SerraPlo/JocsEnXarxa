#pragma once
#include <SerraPloEngine/IScreen.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SerraPloEngine/Camera.h>
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/CarPhysics.h>
#include "RendererList.h"
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

#define MAX_POINT_LIGHTS 1
#define MAX_SPOT_LIGHTS 1

class PlaygroundScreen : public IScreen {
public:
	explicit PlaygroundScreen();
	~PlaygroundScreen();

	virtual void Build() override;
	virtual void Destroy() override;
	virtual void OnEntry() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual int GetNextScreenIndex() const override;
	virtual int GetPrevScreenIndex() const override;
private:
	void checkInput();

	ShaderProgram m_mainProgram;
	ShaderProgram m_lightProgram;
	Camera m_camera;
	RendererList m_renderer;
	GameObject *m_player;
	CarPhysics m_carPhy;
	DirLight m_dirLight;
	PointLight m_pointLights[MAX_POINT_LIGHTS];
	SpotLight m_spotLights[MAX_SPOT_LIGHTS];
};
