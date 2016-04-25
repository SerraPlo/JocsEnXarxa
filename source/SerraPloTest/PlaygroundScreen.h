#pragma once
#include <SerraPloEngine/Camera2D.h>
#include <SerraPloEngine/GLSLManager.h>
#include <SerraPloEngine/DebugRenderer.h>
#include <SerraPloEngine/SpriteBatch.h>
#include <SerraPloEngine/IScreen.h>
#include <SerraPloEngine/Light2D.h>
#pragma comment(lib, "SerraPloEngine.lib")

#define RENDER_DEBUG_MODE

class PlaygroundScreen : public SerraPlo::IScreen
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

	SerraPlo::GLSLManager m_textureProgram;
	SerraPlo::GLSLManager m_lightProgram;
	SerraPlo::Camera2D m_camera;
	SerraPlo::SpriteBatch m_spriteBatch;
	SerraPlo::Light2D* m_mouseLight;
	SerraPlo::DebugRenderer m_debugRenderer;
};
