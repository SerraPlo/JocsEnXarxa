#include "MenuScreen.h"
#include "AppClient.h"
#include <SerraPloEngine\PathLoader.h>

void MenuScreen::Build(void) {
	m_app = dynamic_cast<AppClient*>(gameApp);

	m_title.Load(0, 0, m_app->screenWidth, m_app->screenHeight, GetPathToAsset("images/title.jpg"), m_app->window.SDLWindow, m_app->renderer);

	m_singlePlayer.Load(int(m_app->screenWidth*0.65f), int(m_app->screenHeight*0.5f),
						int(m_app->screenWidth*0.3f), int(m_app->screenHeight*0.2f),
						GetPathToAsset("images/single_player_default.png"),
						GetPathToAsset("images/single_player_hover.png"),
						m_app->window.SDLWindow, m_app->renderer);

	m_multiplayer.Load(int(m_app->screenWidth*0.65f), int(m_app->screenHeight*0.7f),
					   int(m_app->screenWidth*0.3f), int(m_app->screenHeight*0.2f),
					   GetPathToAsset("images/multiplayer_default.png"),
					   GetPathToAsset("images/multiplayer_hover.png"),
						m_app->window.SDLWindow, m_app->renderer);
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
	if (m_multiplayer.pressed) m_app->ChangeScreen((m_app->assetManager.Empty()) ? SCREEN_LOGIN : SCREEN_MULTIPLAYER);
	if (m_app->inputManager.isKeyPressed(SDLK_ESCAPE)) m_app->m_currentScreen->currentState = ScreenState::EXIT;
}

void MenuScreen::Draw(void) {
	SDL_RenderClear(m_app->renderer);
		m_title.Draw(m_app->renderer);
		m_singlePlayer.Draw(m_app->renderer);
		m_multiplayer.Draw(m_app->renderer);
	SDL_RenderPresent(m_app->renderer);
}
