#include "IMainGame.h"
#include "Timing.h"
#include "ScreenList.h"
#include "IGameScreen.h"

namespace SerraPlo {

IMainGame::IMainGame() {
	m_screenList = std::make_unique<ScreenList>(this);
}

void IMainGame::init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	//tell we want a double buffered windows to avoid flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	initSystems();
	onInit();
	addScreens();
	m_currentScreen = m_screenList->getCurScreen();
	m_currentScreen->onEntry();
	m_currentScreen->setRunning();
}

void IMainGame::initSystems() {
	m_window.create("Default", 1600, 900, 0);
}

void IMainGame::update() {
	if (m_currentScreen) {
		switch(m_currentScreen->getState()) {
		case ScreenState::RUNNING:
			m_currentScreen->update();
			break;
		case ScreenState::CHANGE_NEXT:
			m_currentScreen->onExit();
			m_currentScreen = m_screenList->moveNext();
			if(m_currentScreen) {
				m_currentScreen->setRunning();
				m_currentScreen->onEntry();
			}
			break;
		case ScreenState::CHANGE_PREVIOUS:
			m_currentScreen->onExit();
			m_currentScreen = m_screenList->movePrev();
			if (m_currentScreen) {
				m_currentScreen->setRunning();
				m_currentScreen->onEntry();
			}
			break;
		case ScreenState::EXIT_APPLICATION:
			exitGame();
			break;
		default: break;
		}
	}
	else exitGame();
}

void IMainGame::draw() {
	glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());
	if (m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING) {
		m_currentScreen->draw();
	}
}

void IMainGame::onSDLEvent(SDL_Event& evnt) {
	switch (evnt.type) {
	case SDL_QUIT:
		m_currentScreen->setExit();
		break;
	case SDL_MOUSEMOTION:
		m_inputManager.setMouseCoords(static_cast<float>(evnt.motion.x), static_cast<float>(evnt.motion.y));
		break;
	case SDL_KEYDOWN:
		m_inputManager.pressKey(evnt.key.keysym.sym);
		break;
	case SDL_KEYUP:
		m_inputManager.releaseKey(evnt.key.keysym.sym);
		break;
	case SDL_MOUSEBUTTONDOWN:
		m_inputManager.pressKey(evnt.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		m_inputManager.releaseKey(evnt.button.button);
		break;
	}
}

void IMainGame::run() {
	init();
	m_isRunning = true;
	FPSLimiter fpsLimiter;
	fpsLimiter.setTargetFPS(120.0f);

	while(m_isRunning) {
		fpsLimiter.begin();
			m_inputManager.update();
			update();
			if (!m_isRunning) break;
			draw();
			m_fps = fpsLimiter.getFPS();
		fpsLimiter.end();
		m_window.swapBuffer();
	}
}

void IMainGame::exitGame() {
	m_currentScreen->onExit();
	if (m_screenList) {
		m_screenList->destroy();
		m_screenList.reset();
	}
	m_isRunning = false;
}

}
