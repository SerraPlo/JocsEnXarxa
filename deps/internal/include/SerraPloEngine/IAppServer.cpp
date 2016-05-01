#include "IAppServer.h"
#include "Timing.h"

namespace SerraPlo {

	IAppServer::~IAppServer() {
		for (auto client : clientList) if (client.second) delete client.second, client.second = nullptr;
	}

	void IAppServer::Run() {
		Init(); // Call the init everything function
		FPSLimiter fpsLimiter; // Spawn the main instance of the timing limiter
		fpsLimiter.setTargetFPS(TARGET_FPS); // Set the frames per second we whish to have, ideally 60-120

		while (m_isRunning) { // While game is running
			fpsLimiter.begin();			// Init FPS counter
			Update();					// Main update function
			if (!m_isRunning) break;	// Break main game loop if running attribute set to false
			fps = fpsLimiter.m_fps;		// Get the current fps of the class instance
			fpsLimiter.end();			// Calculate and restore FPS
		}
	}

	void IAppServer::ExitGame() {}

}
