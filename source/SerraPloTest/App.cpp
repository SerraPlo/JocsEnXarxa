#include <SerraPloEngine/ScreenList.h>
#include "App.h"

App::App() : IApp(TARGET_FPS) {
}

App::~App() {
}

void App::OnInit() {}

void App::AddScreens() {
	m_testScreen = std::make_unique<PlaygroundScreen>();
	m_screenList->AddScreen(m_testScreen.get());
	m_screenList->SetScreen(m_testScreen->m_screenIndex);
}

void App::OnExit() {}

#undef TARGET_FPS