#pragma once
#include "Window.h"
#include "InputManager.h"
#include <memory>

namespace SerraPlo {

class ScreenList;
class IGameScreen;
class IMainGame
{
protected:
	std::unique_ptr<ScreenList> m_screenList = nullptr;
	IGameScreen* m_currentScreen = nullptr;
	bool m_isRunning = false;
	float m_fps = 0;
	GLWindow m_window;
	InputManager m_inputManager;

	void init();
	void initSystems();

	virtual void update();
	virtual void draw();
public:
	explicit IMainGame();
	virtual ~IMainGame() = default;

	void run();
	void exitGame();

	virtual void onInit() = 0;
	virtual void addScreens() = 0;
	virtual void onExit() = 0;
	void onSDLEvent(SDL_Event &evnt);

	float getFPS() const { return m_fps; };
	InputManager &getInputManager() { return m_inputManager; };
};

}
