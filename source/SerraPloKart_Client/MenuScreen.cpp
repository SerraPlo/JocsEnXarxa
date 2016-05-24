#include "MenuScreen.h"
#include "AppClient.h"
#include <SerraPloEngine\ResourceManager.h>

#define INIT_SPRITE(sprite, path, x, y) sprite.Load(LoadAsset(path), m_app->window.SDLWindow, m_app->renderer); sprite.position = { x,y };

void MenuScreen::Build(void) {
	m_app = dynamic_cast<AppClient*>(gameApp);

	INIT_SPRITE(m_title, "images/title.jpg", 0, 0);
	INIT_SPRITE(m_singlePlayer, "images/single_player.jpg", m_app->screenWidth*0.6f, m_app->screenHeight / 2);
}

void MenuScreen::Destroy(void) {
	
}

void MenuScreen::OnEntry(void) {
	SDL_SetRenderDrawColor(m_app->renderer, 255, 255, 255, 255);
	SDL_StartTextInput();
}

void MenuScreen::OnExit(void) {
	SDL_StopTextInput();
}

void MenuScreen::Update(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt)) m_app->OnSDLEvent(evnt);
	m_singlePlayer.Update(m_app->inputManager);
	if (m_singlePlayer.pressed) m_app->ChangeScreen(LOGIN_SCREEN);
}

void MenuScreen::Draw(void) {
	SDL_RenderClear(m_app->renderer);
	m_title.Draw(m_app->renderer);
	m_singlePlayer.Draw(m_app->renderer);
	SDL_RenderPresent(m_app->renderer);
}
