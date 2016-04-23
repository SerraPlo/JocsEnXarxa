#include <SerraPloEngine/ScreenList.h>
#include "App.h"

App::App() {
}

App::~App() {
}

void App::onInit() {}

void App::addScreens() {
	m_testScreen = std::make_unique<PlaygroundScreen>();
	m_screenList->addScreen(m_testScreen.get());
	m_screenList->setScreen(m_testScreen->getScreenIndex());
}

void App::onExit() {}
