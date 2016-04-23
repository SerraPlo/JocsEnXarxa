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

	virtual void build() override;
	virtual void destroy() override;
	virtual void onEntry() override;
	virtual void onExit() override;
	virtual void update() override;
	virtual void draw() override;
	virtual int getNextScreenIndex() const override;
	virtual int getPrevScreenIndex() const override;
private:
	void checkInput() const;

	SerraPlo::GLSLManager m_textureProgram;
	SerraPlo::GLSLManager m_lightProgram;
	SerraPlo::Camera2D m_camera;
	SerraPlo::SpriteBatch m_spriteBatch;
	SerraPlo::Light2D* m_mouseLight;
	SerraPlo::DebugRenderer m_debugRenderer;
};
