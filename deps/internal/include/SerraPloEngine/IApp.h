#pragma once
#include "Window.h"
#include "InputManager.h"
#include <memory>

namespace SerraPlo {

	class ScreenList;	// Forward declaration of the list of screens
	class IScreen;		// Forward declaration of the game screen interface

	// Game app interface to the store the main runing game engine as a template
	class IApp {
	protected:
		std::unique_ptr<ScreenList> m_screenList;	// Unique pointer instance to the list of screens of the game
		IScreen *m_currentScreen;					// Reference pointer to the screen running at the moment
		bool m_isRunning;							// Whether game is running or not

		// Initialize everything related to game internals
		void Init();
		// Initialize SDKs, window, audio, events...
		void InitSystems();

		// Main update function of the game
		virtual void Update();
		// Main draw function of the game
		virtual void Draw();
	public:
		float const targetFPS;		// How many fps do we wish to have
		float fps;					// How many frames per second the game is running to
		GLWindow window;				// Main instance of the OpenGL window
		InputManager inputManager;	// Main instance of the input manager class

		explicit IApp(float tfps);
		virtual ~IApp() = default;

		// Manage main SDL event types
		void OnSDLEvent(SDL_Event &evnt);

		// Where magic occurs, to be used to play the whole game
		void Run();
		// Destroy screen list and set game running to false
		void ExitGame();

		// Initialize specific game attributes in derived app class
		virtual void OnInit() = 0;
		// Push and store the app screens
		virtual void AddScreens() = 0;
		// Destroy specific game attributes in derived app class
		virtual void OnExit() = 0;
	};

}
