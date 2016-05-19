#pragma once
#include <SerraPloEngine/IApp.h>
#include <SerraPloEngine/Window.h>
#include <SerraPloEngine/InputManager.h>
#include <SerraPloEngine/ScreenList.h>
#include <SerraPloEngine/IScreen.h>
#include <SerraPloEngine/GameObjectManager.h>
#include <UDPStream/UDPStream.hh>
#include <memory>
#include "PlaygroundScreen.h"

#define INIT_SCREEN_WIDTH 1200
#define INIT_SCREEN_HEIGHT 900
#define IP_PORT "127.0.0.1:5000"

class AppClient : public IApp {
	std::unique_ptr<ScreenList> m_screenList{ std::make_unique<ScreenList>(this) };	// Unique pointer instance to the list of screens of the game
	IScreen *m_currentScreen{ nullptr };											// Reference pointer to the screen running at the moment

	explicit AppClient() : serverAddress(IP_PORT) {};
	AppClient(AppClient const&) = delete;
	void operator=(AppClient const&) = delete;
	~AppClient() = default;

	// Initialize everything related to game internals
	virtual void Init() override;
	// Main update function of the game
	void Update();
	// Main draw function of the game
	void Draw() const;
public:
	int screenWidth{ INIT_SCREEN_WIDTH }, screenHeight{ INIT_SCREEN_HEIGHT };
	UDPStream mainSocket;
	sockaddr serverAddress;
	GLWindow window;			// Main instance of the OpenGL window
	InputManager inputManager;	// Main instance of the input manager class
	GameObjectManager gameObjectManager;
	std::string nick{ " " };

	static AppClient& Instance() {
		static AppClient instance;
		return instance;
	}

	// Manage main SDL event types
	void OnSDLEvent(SDL_Event &evnt);
	// Where magic occurs, to be used to play the whole game
	virtual void Run() override;
	// Destroy screen list and set game running to false
	virtual void Exit() override;
};