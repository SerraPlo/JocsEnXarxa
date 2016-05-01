#include <SerraPloEngine/ScreenList.h>
#include "AppClient.h"

void AppClient::OnInit() {
	window.changeName("SerraPlo Kart Client");
}

void AppClient::AddScreens() {
	m_testScreen = std::make_unique<PlaygroundScreen>();
	m_screenList->AddScreen(m_testScreen.get());
	m_screenList->SetScreen(m_testScreen->screenIndex);
}

void AppClient::OnExit() {}

#undef TARGET_FPS