#include <SerraPloEngine/ScreenList.h>
#include <SerraPloEngine/ResourceManager.h>
#include "AppClient.h"

/*"character_bb8": {
"name": "BB8",
"description": "ahjsdfhsjfha",
"model": "models/bb8/bb8.obj",
"texture": "models/bb8/bb8_diffuse.jpg",
"position": [6, 0, 8],
"rotation": [0, 4, 0],
"scale": [3, 1, 1]
}*/

void AppClient::OnInit() {
	window.changeName("SerraPlo Kart Client");
	gameObjectManager.Load(LoadAsset("json/characters.json"));
}

void AppClient::AddScreens() {
	m_testScreen = std::make_unique<PlaygroundScreen>();
	m_screenList->AddScreen(m_testScreen.get());
	m_screenList->SetScreen(m_testScreen->screenIndex);
}

void AppClient::OnExit() {}

#undef TARGET_FPS