#pragma once

namespace SerraPlo {

#define TARGET_FPS 120.0f
#define MS_ALIVE_DELAY 5000
#define MS_RESEND_DELAY 4000

	enum KEY_MESSAGE {
		MSG_LOGIN, MSG_BEGIN, MSG_EXIT, MSG_ALIVE, MSG_UPDATE, MSG_NEW_PLAYER
	};

	struct input10 {
		bool w[10];
		bool a[10];
		bool s[10];
		bool d[10];
		float dt[10];
	};

	// Game app interface to the store the main runing game engine as a template
	class IApp {
	protected:
		bool m_isRunning {true};			// Whether game is running or not
		float m_aliveCounter {0.0f};
	public:
		float fps{ 0 };			// How many frames per second the game is running to
		float deltaTime{ 0 };	// Difference between last frame and new frame

		explicit IApp() = default;
		virtual ~IApp() = default;

		// Initialize everything related to game internals
		virtual void Init(void) = 0;
		// Where magic occurs, to be used to play the whole game
		virtual void Run(void) = 0;
	};

}
