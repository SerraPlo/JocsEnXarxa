#pragma once

namespace SerraPlo {

#define TARGET_FPS 120.0f

	enum KEY_MESSAGE {
		LOGIN, MOVE
	};

	// Game app interface to the store the main runing game engine as a template
	class IApp {
	protected:
		bool m_isRunning {true};			// Whether game is running or not
	public:
		float fps{ 0 };			// How many frames per second the game is running to
		float deltaTime{ 0 };	// Difference between last frame and new frame

		explicit IApp() = default;
		virtual ~IApp() = default;

		// Initialize everything related to game internals
		virtual void Init() = 0;
		// Where magic occurs, to be used to play the whole game
		virtual void Run() = 0;
		// Destroy screen list and set game running to false
		virtual void Exit() = 0;
	};

}
