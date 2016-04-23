#pragma once
#include "Window.h"
#include "InputManager.h"
#include <memory>

namespace SerraPlo {

class ScreenList;
class IScreen;
class IApp
{
protected:
	std::unique_ptr<ScreenList> m_screenList = nullptr;
	IScreen* m_currentScreen = nullptr;
	bool m_isRunning = false;

	void init();
	void initSystems();

	virtual void update();
	virtual void draw();
public:
	float m_fps = 0;
	GLWindow m_window;
	InputManager m_inputManager;

	explicit IApp();
	virtual ~IApp() = default;

	void run();
	void exitGame();

	virtual void onInit() = 0;
	virtual void addScreens() = 0;
	virtual void onExit() = 0;
	void onSDLEvent(SDL_Event &evnt);
};

}
