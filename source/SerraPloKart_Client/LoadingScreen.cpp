#include "LoadingScreen.h"
#include "AppClient.h"
#include <ctime>

void LoadingScreen::Build(void) {
	m_app = dynamic_cast<AppClient*>(gameApp);

	m_loginMessage.Load("Please enter a nickname:", { 255,255,255 }, m_app->window.SDLWindow, m_app->renderer, m_app->font);
	m_loginMessage.width = int(m_app->screenWidth*0.9f);
	m_loginMessage.height = int(m_app->screenHeight*0.2f);
	m_loginMessage.position = { m_app->screenWidth/2- m_loginMessage.width/2, m_app->screenHeight*0.3f };

	m_loadMessage.Load("Loading... Please wait", { 170,0,0 }, m_app->window.SDLWindow, m_app->renderer, m_app->font);
	m_loadMessage.width = int(m_app->screenWidth*0.9f);
	m_loadMessage.height = int(m_app->screenHeight*0.2f);
	m_loadMessage.position = { m_app->screenWidth / 2 - m_loadMessage.width / 2, m_app->screenHeight / 2 - m_loadMessage.height / 2 };
}

void LoadingScreen::Destroy(void) {

}

void LoadingScreen::OnEntry(void) {
	SDL_SetRenderDrawColor(m_app->renderer, 0, 0, 0, 255);
	SDL_StartTextInput();
}

void LoadingScreen::OnExit(void) {
	SDL_StopTextInput();
}

void LoadingScreen::Update(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt)) m_app->OnSDLEvent(evnt);
	if (m_app->multiplayerMode) {
		if (m_app->nick.empty()) {
			if (SDL_PollEvent(&evnt)) {
				switch (evnt.type) {
					case SDL_TEXTINPUT: if (m_nickText.size() < TEXT_MAX_SIZE) m_nickText += evnt.text.text; break;
					//case SDL_TEXTEDITING: break;
				}
			}
			if (m_app->inputManager.isKeyPressed(SDLK_RETURN) && !m_nickText.empty()) m_app->nick = m_nickText;
			if (m_app->inputManager.isKeyPressed(SDLK_BACKSPACE)) m_nickText = m_nickText.substr(0, m_nickText.size() - 1);

			m_userInput.Load(m_nickText, { 140,0,140 }, m_app->window.SDLWindow, m_app->renderer, m_app->font);
			m_userInput.width = int(m_app->screenWidth*0.001f*m_nickText.size()*FONT_SIZE);
			m_userInput.height = int(m_app->screenHeight*0.2f);
			m_userInput.position = { m_app->screenWidth / 2 - (m_userInput.width / 2), m_app->screenHeight* 0.6f };
		} else {
			if (m_app->assetManager.Empty()) m_app->LoadAssets();
			try {
				if (clock() > m_counterSend + MS_RESEND_DELAY) m_counterSend = float(clock()), m_app->ChangeScreen(SCREEN_MULTIPLAYER),
					m_app->mainSocket << UDPStream::packet << MSG_LOGIN << m_app->nick << m_app->serverAddress,
					std::cout << "Nick sent. Waiting server response..." << std::endl;
			} catch (UDPStream::wrong) { //if the amount of packet data not corresponding to the amount of data that we are trying to read
				std::cout << "--> ALERT: Wrongly serialized data received!" << std::endl;
			} catch (UDPStream::empty) {} //if the package is empty or have not received anything
		}
	} else {
		if (m_app->assetManager.Empty()) m_app->LoadAssets();
		m_app->ChangeScreen(SCREEN_SINGLE_PLAYER);
	}
	
	if (m_app->inputManager.isKeyPressed(SDLK_ESCAPE)) m_app->currentScreen->currentState = ScreenState::EXIT;
}

void LoadingScreen::Draw(void) {
	SDL_RenderClear(m_app->renderer);
		(m_app->multiplayerMode) ? (m_app->nick.empty()) ? m_loginMessage.Draw(m_app->renderer), m_userInput.Draw(m_app->renderer) : m_loadMessage.Draw(m_app->renderer) :
									m_loadMessage.Draw(m_app->renderer);
	SDL_RenderPresent(m_app->renderer);
}
