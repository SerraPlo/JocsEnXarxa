#pragma once
#include "Window.h"
#include "InputManager.h"
#include <memory>
#include "IApp.h"
#include <UDPStream/UDPStream.hh>

namespace SerraPlo {

	class ScreenList;	// Forward declaration of the list of screens
	class IScreen;		// Forward declaration of the game screen interface

	class IAppClient : public IApp {
	protected:
		std::unique_ptr<ScreenList> m_screenList;	// Unique pointer instance to the list of screens of the game
		IScreen *m_currentScreen;					// Reference pointer to the screen running at the moment
		
		// Initialize SDKs, window, audio, events...
		void InitSystems();
		// Initialize everything related to game internals
		virtual void Init() override;
		// Main update function of the game
		virtual void Update() override;
		// Main draw function of the game
		virtual void Draw();
		
	public:
		UDPStream client;
		sockaddr serverAddress;
		GLWindow window;			// Main instance of the OpenGL window
		InputManager inputManager;	// Main instance of the input manager class

		explicit IAppClient(const char* ipport);
		~IAppClient();

		// Manage main SDL event types
		void OnSDLEvent(SDL_Event &evnt);

		// Where magic occurs, to be used to play the whole game
		virtual void Run() override;
		// Destroy screen list and set game running to false
		virtual void ExitGame() override;
	};

}

