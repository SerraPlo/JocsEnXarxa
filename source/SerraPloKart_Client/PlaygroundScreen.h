#pragma once
#include <SerraPloEngine/Camera2D.h>
#include <SerraPloEngine/GLSLManager.h>
#include <SerraPloEngine/DebugRenderer.h>
#include <SerraPloEngine/SpriteBatch.h>
#include <SerraPloEngine/IScreen.h>
#include <SerraPloEngine/Light2D.h>
#include <SerraPloEngine/ParticleBatch2D.h>
#include <SerraPloEngine/ParticleEngine2D.h>
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

#define RENDER_DEBUG_MODE

class PlaygroundScreen : public IScreen
{
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
	void checkInput() const;

	GLSLManager m_textureProgram;
	GLSLManager m_lightProgram;
	Camera2D m_camera;
	SpriteBatch m_spriteBatch;
	Light2D *m_mouseLight;
	ParticleBatch2D *m_particleBatch;
	ParticleEngine2D m_particleEngine;
	DebugRenderer m_debugRenderer;
};
