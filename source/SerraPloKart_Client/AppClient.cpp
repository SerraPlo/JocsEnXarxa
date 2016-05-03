#include <SerraPloEngine/ScreenList.h>
#include <SerraPloEngine/ResourceManager.h>
#include "AppClient.h"

void AppClient::OnInit() {
	window.changeName("SerraPlo Kart Client");
	m_entityManager.LoadJson<Character>(ResourceManager::LoadAsset("json/characters.json"));
}

void AppClient::AddScreens() {
	m_testScreen = std::make_unique<PlaygroundScreen>();
	m_screenList->AddScreen(m_testScreen.get());
	m_screenList->SetScreen(m_testScreen->screenIndex);
}

void AppClient::OnExit() {}

#undef TARGET_FPS