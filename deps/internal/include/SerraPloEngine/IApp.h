#pragma once

namespace SerraPlo {

#define TARGET_FPS 120.0f

	// Game app interface to the store the main runing game engine as a template
	class IApp {
	protected:
		bool m_isRunning {true};			// Whether game is running or not

		// Initialize everything related to game internals
		virtual void Init() = 0;
		// Main update function of the game
		virtual void Update() = 0;
	public:
		float fps{ 0 };						// How many frames per second the game is running to

		explicit IApp() = default;
		virtual ~IApp() = default;

		// Where magic occurs, to be used to play the whole game
		virtual void Run() = 0;
		// Destroy screen list and set game running to false
		virtual void ExitGame() = 0;

		// Initialize specific game attributes in derived app class
		virtual void OnInit() = 0;
		// Push and store the app screens
		virtual void AddScreens() = 0;
		// Destroy specific game attributes in derived app class
		virtual void OnExit() = 0;
	};

}
