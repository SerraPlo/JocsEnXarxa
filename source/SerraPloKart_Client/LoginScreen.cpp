#include "LoginScreen.h"
#include "AppClient.h"
#include <SerraPloEngine\ResourceManager.h>
#include <ctime>

void LoginScreen::Build(void) {
	m_app = dynamic_cast<AppClient*>(gameApp);

	m_loginMessage.Load("Please enter a nickname:", { 255,255,255 }, m_app->window.SDLWindow, m_app->renderer, m_app->font);
	m_loginMessage.position = { m_app->screenWidth/2- m_loginMessage.width/2, m_app->screenHeight*0.3f };

	m_loadMessage.Load("Loading... Please wait", { 170,0,0 }, m_app->window.SDLWindow, m_app->renderer, m_app->font);
	m_loadMessage.position = { m_app->screenWidth / 2 - m_loadMessage.width / 2, m_app->screenHeight / 2 - m_loadMessage.height / 2 };

	//m_userInput.Load(m_nickText, { 100,0,100 }, m_app->window.SDLWindow, m_app->renderer, m_app->font);
	//m_userInput.position = { m_app->screenWidth / 2 - m_message.width / 2, m_app->screenHeight / 2 * 0.6f };
}

void LoginScreen::Destroy(void) {

}

void LoginScreen::OnEntry(void) {
	SDL_SetRenderDrawColor(m_app->renderer, 0, 0, 0, 255);
	SDL_StartTextInput();
}

void LoginScreen::OnExit(void) {
	SDL_StopTextInput();
}

void LoginScreen::Update(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt)) m_app->OnSDLEvent(evnt);
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
		m_userInput.position = { m_app->screenWidth / 2 - (m_userInput.width / 2)*1.25f, m_app->screenHeight* 0.5f };
	} else {
		static float counterSend = 0;
		if (!m_app->gameAssetsLoaded) m_app->gameObjectManager.Load(LoadAsset("gameObjects.json")), m_app->gameAssetsLoaded = true;
		try {
			if (clock() > counterSend + MS_RESEND_DELAY) counterSend = clock(), m_app->ChangeScreen(MULTIPLAYER_SCREEN),
				m_app->mainSocket << UDPStream::packet << LOGIN << m_app->nick << m_app->serverAddress, 
				std::cout << "Nick sent. Waiting server response..." << std::endl;
		} catch (UDPStream::wrong) { //if the amount of packet data not corresponding to the amount of data that we are trying to read
			std::cout << "--> ALERT: Wrongly serialized data received!" << std::endl;
		} catch (UDPStream::empty) {} //if the package is empty or have not received anything
	}
	if (m_app->inputManager.isKeyPressed(SDLK_ESCAPE)) m_app->m_currentScreen->currentState = ScreenState::EXIT;
}

void LoginScreen::Draw(void) {
	SDL_RenderClear(m_app->renderer);
		(m_app->nick.empty()) ? m_loginMessage.Draw(m_app->renderer), m_userInput.Draw(m_app->renderer, int(40 * m_nickText.size())) :
								m_loadMessage.Draw(m_app->renderer);
	SDL_RenderPresent(m_app->renderer);
}
