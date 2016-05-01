#include <SerraPloEngine/ScreenList.h>
#include "App.h"

AppClient::AppClient() : IApp(TARGET_FPS) {
}

AppClient::~AppClient() {
}

void AppClient::OnInit() {}

void AppClient::AddScreens() {
	m_testScreen = std::make_unique<PlaygroundScreen>();
	m_screenList->AddScreen(m_testScreen.get());
	m_screenList->SetScreen(m_testScreen->screenIndex);
}

void AppClient::OnExit() {}

#undef TARGET_FPS