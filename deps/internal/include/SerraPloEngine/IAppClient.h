#pragma once
#include "IApp.h"
#include "Window.h"
#include "InputManager.h"
#include "ScreenList.h"
#include "IScreen.h"
#include <memory>
#include <UDPStream/UDPStream.hh>
#include "GameObjectManager.h"

namespace SerraPlo {

	//class ScreenList;	// Forward declaration of the list of screens
	//class IScreen;		// Forward declaration of the game screen interface

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
		void Draw() const;
		
	public:
		int screenWidth, screenHeight;
		UDPStream mainSocket;
		sockaddr serverAddress;
		GLWindow window;			// Main instance of the OpenGL window
		InputManager inputManager;	// Main instance of the input manager class
		GameObjectManager gameObjectManager;
		std::string nick{" "};

		explicit IAppClient(int sw, int sh, const char* ipport);
		virtual ~IAppClient() = default;

		// Manage main SDL event types
		void OnSDLEvent(SDL_Event &evnt);

		// Where magic occurs, to be used to play the whole game
		virtual void Run() override;
		// Destroy screen list and set game running to false
		virtual void ExitGame() override;

		// Initialize specific game attributes in derived app class
		virtual void OnInit() = 0;
		// Push and store the app screens
		virtual void AddScreens() = 0;
		// Destroy specific game attributes in derived app class
		virtual void OnExit() = 0;
	};

}

