#pragma once
#include <SerraPloEngine/IApp.h>
#include <SerraPloEngine/Window.h>
#include <SerraPloEngine/InputManager.h>
#include <SerraPloEngine/ScreenList.h>
#include <SerraPloEngine/GameObjectManager.h>
#include <UDPStream/UDPStream.hh>
#include <memory>
#include "MenuScreen.h"
#include "LoginScreen.h"
#include "MultiplayerScreen.h"

#define INIT_SCREEN_WIDTH 1200
#define INIT_SCREEN_HEIGHT 900
#define IP_PORT "127.0.0.1:5000"
#define FONT_SIZE 70

enum SreenType { MENU_SCREEN, LOGIN_SCREEN, MULTIPLAYER_SCREEN, SINGLE_PLAYER_SCREEN };

class AppClient : public IApp {
	std::unique_ptr<ScreenList> m_screenList{ std::make_unique<ScreenList>(dynamic_cast<IApp*>(this)) }; // Unique pointer instance to the list of screens of the game
	///TODO: optimize to local
	std::unique_ptr<MenuScreen> m_menuScreen;
	std::unique_ptr<LoginScreen> m_loginScreen;
	std::unique_ptr<MultiplayerScreen> m_gameplayScreen;

	explicit AppClient() : serverAddress(IP_PORT) {};
	AppClient(AppClient const&) = delete;
	void operator=(AppClient const&) = delete;
	~AppClient() = default;

	// Initialize everything related to game internals
	virtual void Init(void) override;
	// General function to receive messages from server
	void ProcessMsgs(void);
	// Main update function of the game
	void Update(void);
	// Main draw function of the game
	void Draw(void) const;
public:
	int screenWidth{ INIT_SCREEN_WIDTH }, screenHeight{ INIT_SCREEN_HEIGHT };
	IScreen *m_currentScreen{ nullptr }; // Reference pointer to the screen running at the moment
	UDPStream mainSocket;
	sockaddr serverAddress;
	GLWindow window;			// Main instance of the OpenGL window
	InputManager inputManager;	// Main instance of the input manager class
	GameObjectManager gameObjectManager;
	std::string nick{ "" };
	SDL_Renderer *renderer{ nullptr };
	TTF_Font *font{ nullptr };
	bool gameAssetsLoaded{ false };

	static AppClient& Instance() {
		static AppClient instance;
		return instance;
	}
	void ChangeScreen(int index);
	// Manage main SDL event types
	void OnSDLEvent(SDL_Event &evnt);
	// Where magic occurs, to be used to play the whole game
	virtual void Run(void) override;
	// Destroy screen list and set game running to false
	virtual void Exit(void);
};