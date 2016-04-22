#pragma once
#include <SerraPloEngine/Camera2D.h>
#include <SerraPloEngine/GLSLManager.h>
#include <SerraPloEngine/DebugRenderer.h>
#include <SerraPloEngine/SpriteBatch.h>
#include <SerraPloEngine/IGameScreen.h>
#include <SerraPloEngine/Window.h>
#include <SerraPloEngine/Light.h>
#pragma comment(lib, "SerraPloEngine.lib")

#define RENDER_DEBUG_MODE

class GameplayScreen : public SerraPlo::IGameScreen
{
public:
	explicit GameplayScreen(SerraPlo::GLWindow* window);
	~GameplayScreen();

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
	SerraPlo::GLWindow* m_window = nullptr;
	SerraPlo::Light* m_mouseLight;
	SerraPlo::DebugRenderer m_debugRenderer;
};
