#include "IAppClient.h"
#include "Timing.h"

namespace SerraPlo {
	
	IAppClient::IAppClient(int sw, int sh, const char* ipport) :
		m_screenList(std::make_unique<ScreenList>(this)),
		m_currentScreen(nullptr),
		screenWidth(sw),
		screenHeight(sh),
		serverAddress(ipport)
	{}

	void IAppClient::InitSystems() {
		SDL_Init(SDL_INIT_EVERYTHING); // Initialize everything in SDL (VIDEO, AUDIO, EVENTS,...)

		// Tell we want a double buffered windows to avoid flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		const SDL_MessageBoxButtonData buttons[] = { { 0, 0, "NO" }, { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "YES" } };
		const SDL_MessageBoxColorScheme colorScheme = { { { 255,   0,   0 }, { 0, 255,   0 }, { 255, 255,   0 }, { 0,   0, 255 }, { 255,   0, 255 } } };
		const SDL_MessageBoxData messageboxdata = { SDL_MESSAGEBOX_INFORMATION, nullptr, "Select an option", "Do you want to play on full screen mode?", SDL_arraysize(buttons), buttons, &colorScheme };
		int buttonid;
		SDL_ShowMessageBox(&messageboxdata, &buttonid); // Whether to play on fullscreen mode or default normal mode

		if (buttonid == 0) window.create("Default", &screenWidth, &screenHeight, WindowFlags::RESIZABLE); // Create default window resizable
		else window.create("Default", &screenWidth, &screenHeight, WindowFlags::FULLSCREEN); // Create default window fullscreen
	}

	void IAppClient::Init() {
		InitSystems();	// Initialize game systems
		OnInit();		// Initialize specific derived properties
		AddScreens();	// Add the screens of the derived app into the list
		m_currentScreen = m_screenList->GetCurScreen(); // Set the current screen reference
		m_currentScreen->OnEntry(); // Initialize the first screen when enter
		m_currentScreen->currentState = ScreenState::RUNNING; // Then set the screen to the running state
	}

#define CHANGE_TO(MoveTo)	m_currentScreen->OnExit(); /* Call the leaving method of the current screen */ \
							m_currentScreen = m_screenList->MoveTo(); /* Set the current screen to the next one in the list */ \
							if (m_currentScreen) { /* If the new screen exists */ \
								m_currentScreen->currentState = ScreenState::RUNNING; /* Set the state of the new screen to running */ \
								m_currentScreen->OnEntry(); /* Then call the function to initialize the scene */ \
							}

	void IAppClient::Update() {
		if (m_currentScreen) { // If current screen exists
			switch (m_currentScreen->currentState) { // Check for the state of the screen
				case ScreenState::RUNNING:
					if (inputManager.isKeyDown(SDLK_ESCAPE)) m_currentScreen->currentState = ScreenState::EXIT_APP;
					inputManager.update();	// Update the input manager instance
					m_currentScreen->Update(); // Update the current screen if running
					break;
				case ScreenState::CHANGE_NEXT:
					CHANGE_TO(MoveNext);
					break;
				case ScreenState::CHANGE_PREVIOUS:
					CHANGE_TO(MovePrev);
					break;
				case ScreenState::EXIT_APP:
					ExitGame(); // Call exit function to end the execution
					break;
				default: break;
			}
		} else ExitGame(); // Call exit function if screen doesn't exist
	}

	void IAppClient::Draw() const {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer
		if (m_currentScreen && m_currentScreen->currentState == ScreenState::RUNNING) { // If screen object exists and its state is running
			m_currentScreen->Draw(); // Then call the draw method of the screen
		}
	}
	

	void IAppClient::Run() {
		Init(); // Call the init everything function
		FPSLimiter fpsLimiter; // Spawn the main instance of the timing limiter
		fpsLimiter.setTargetFPS(TARGET_FPS); // Set the frames per second we whish to have, ideally 60-120

		while (m_isRunning) { // While game is running
			fpsLimiter.begin();					// Init FPS counter
			Update();							// Main update function
			if (!m_isRunning) break;			// Break main game loop if running attribute set to false
			Draw();								// Main draw function
			fps = fpsLimiter.fps;				// Get the current fps of the class instance
			deltaTime = fpsLimiter.deltaTime;			// Get the current fps of the class instance
			fpsLimiter.end();					// Calculate and restore FPS
			window.swapBuffer();				// Swap OpenGL buffers if double-buffering is supported
		}
	}

	void IAppClient::ExitGame() {
		m_currentScreen->OnExit(); // Call the leaving method of the current screen
		if (m_screenList) { // Check if pointer to screen list exists
			m_screenList->Destroy(); // Then destroy the screen list
			m_screenList.reset(); // Restore the pointer set to nullptr
		}
		m_isRunning = false; // Execution ends
	}

	void IAppClient::OnSDLEvent(SDL_Event & evnt) {
		switch (evnt.type) { // Check for SDL event type
			case SDL_QUIT:
				m_currentScreen->currentState = ScreenState::EXIT_APP; // Set screen state to exit application
				break;
			case SDL_MOUSEMOTION:
				inputManager.m_mouseCoords = { static_cast<float>(evnt.motion.x), static_cast<float>(evnt.motion.y) }; // Store the mouse coordinates each time mouse moves through the screen
				break;
			case SDL_KEYDOWN:
				inputManager.pressKey(evnt.key.keysym.sym); // Store which key has been pressed
				break;
			case SDL_KEYUP:
				inputManager.releaseKey(evnt.key.keysym.sym); // Store which key has been released
				break;
			case SDL_MOUSEBUTTONDOWN:
				inputManager.pressKey(evnt.button.button); // Store when mouse button is pressed
				break;
			case SDL_MOUSEBUTTONUP:
				inputManager.releaseKey(evnt.button.button); // Store when mouse button is released
				break;
			case SDL_MOUSEWHEEL:
				inputManager.zoom = evnt.wheel.y;
				break;
		}
	}

}


