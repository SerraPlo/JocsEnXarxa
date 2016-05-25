#include "MenuScreen.h"
#include "AppClient.h"
#include <SerraPloEngine\ResourceManager.h>

void MenuScreen::Build(void) {
	m_app = dynamic_cast<AppClient*>(gameApp);

	m_title.Load(LoadAsset("images/title.jpg"), m_app->window.SDLWindow, m_app->renderer);
	m_title.position = { 0,0 };

	m_singlePlayer.Load(LoadAsset("images/single_player_default.png"), 
						LoadAsset("images/single_player_hover.png"),
						m_app->window.SDLWindow, m_app->renderer);
	m_singlePlayer.position = { m_app->screenWidth*0.65f, m_app->screenHeight*0.5f };

	m_multiplayer.Load(LoadAsset("images/multiplayer_default.png"), 
					   LoadAsset("images/multiplayer_hover.png"),
						m_app->window.SDLWindow, m_app->renderer);
	m_multiplayer.position = { m_app->screenWidth*0.65f, m_app->screenHeight*0.7f };
}

void MenuScreen::Destroy(void) {
	
}

void MenuScreen::OnEntry(void) {
	SDL_SetRenderDrawColor(m_app->renderer, 255, 255, 255, 255);
	m_singlePlayer.Reset();
	m_multiplayer.Reset();
}

void MenuScreen::OnExit(void) {

}

void MenuScreen::Update(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt)) m_app->OnSDLEvent(evnt);
	m_singlePlayer.Update(m_app->inputManager);
	m_multiplayer.Update(m_app->inputManager);
	if (m_multiplayer.pressed) m_app->ChangeScreen((!m_app->gameAssetsLoaded) ? LOGIN_SCREEN : MULTIPLAYER_SCREEN);
	if (m_app->inputManager.isKeyPressed(SDLK_ESCAPE)) m_app->m_currentScreen->currentState = ScreenState::EXIT;
}

void MenuScreen::Draw(void) {
	SDL_RenderClear(m_app->renderer);
		m_title.Draw(m_app->renderer);
		m_singlePlayer.Draw(m_app->renderer);
		m_multiplayer.Draw(m_app->renderer);
	SDL_RenderPresent(m_app->renderer);
}