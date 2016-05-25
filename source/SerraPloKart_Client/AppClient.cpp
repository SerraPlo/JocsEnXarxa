#include <SerraPloEngine/ResourceManager.h>
#include <SerraPloEngine/Utils.h>
#include <SerraPloEngine/Timing.h>
#include <SerraPloEngine/IScreen.h>
#include "AppClient.h"
#include <ctime>

void AppClient::Init(void) {
	InitSDL(); // Initialize everything related to SDL for the window
	unsigned int flags = 0;
	//if (AskUserForWindow() == 0) flags = WindowFlags::RESIZABLE; // Create default window resizable
	//else flags = WindowFlags::FULLSCREEN; // Create default window fullscreen
	window.create("SerraPlo Kart Client", &screenWidth, &screenHeight, flags);
	renderer = SDL_CreateRenderer(window.SDLWindow, 0, SDL_RENDERER_ACCELERATED);
	font = TTF_OpenFont(LoadAsset("fonts/ARIAL.TTF").c_str(), FONT_SIZE);
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	InitOpenGL(); // Initialize OpenGL systems after GLWindow creation

	// Add the screens of the derived app into the list
	m_menuScreen = std::make_unique<MenuScreen>();
	m_screenList->AddScreen(m_menuScreen.get(), MENU_SCREEN);
	m_loginScreen = std::make_unique<LoginScreen>();
	m_screenList->AddScreen(m_loginScreen.get(), LOGIN_SCREEN);
	m_gameplayScreen = std::make_unique<MultiplayerScreen>();
	m_screenList->AddScreen(m_gameplayScreen.get(), MULTIPLAYER_SCREEN);
	m_currentScreen = m_screenList->SetScreen(MENU_SCREEN);

	m_aliveCounter = float(clock());
}

void AppClient::ChangeScreen(int index) {
	m_currentScreen = m_screenList->SetScreen(index);
}

void AppClient::OnSDLEvent(SDL_Event & evnt) {
	switch (evnt.type) { // Check for SDL event type
		case SDL_QUIT:
		m_currentScreen->currentState = ScreenState::EXIT; // Set screen state to exit application
		break; case SDL_MOUSEMOTION:
		inputManager.mouseCoords = { static_cast<float>(evnt.motion.x), static_cast<float>(evnt.motion.y) }; // Store the mouse coordinates each time mouse moves through the screen
		break; case SDL_KEYDOWN:
		inputManager.pressKey(evnt.key.keysym.sym); // Store which key has been pressed
		break; case SDL_KEYUP:
		inputManager.releaseKey(evnt.key.keysym.sym); // Store which key has been released
		break; case SDL_MOUSEBUTTONDOWN:
		inputManager.pressKey(evnt.button.button); // Store when mouse button is pressed
		break; case SDL_MOUSEBUTTONUP:
		inputManager.releaseKey(evnt.button.button); // Store when mouse button is released
		break; case SDL_MOUSEWHEEL:
		inputManager.zoom = evnt.wheel.y;
	}
}

void AppClient::ProcessMsgs(void) {
	try {
		int header;
		mainSocket >> UDPStream::packet >> header;
		switch (header) {
			case BEGIN: {
				std::cout << "Server accepted entry. Game begins." << std::endl; 
				//ChangeScreen(MULTIPLAYER_SCREEN);
			} break;
			case EXIT: {
				std::cout << "Server closed. Disconecting..." << std::endl;
				Exit();
			} break;
			case ALIVE: {
				std::cout << "Server is alive" << std::endl;
				m_aliveCounter = float(clock());
			} break;
			case UPDATE: {
				Enemy a;
				mainSocket >> a.nick >> a.transform.position.x >> a.transform.position.z >> a.transform.rotation.y;
				std::cout << a.nick << std::endl;
				if (a.nick == nick);//mirar trampes TODo
				else {
					bool virgin = true; ///TODOOOOOOOOOOOOOOOOOO
					for (auto &enemy : enemies) {
						if (enemy.nick == a.nick) {
							enemy.transform = a.transform;
							virgin = false;
							break;
						} 
					}
					if(virgin) enemies.push_back(a);
				}
			} break;
			default: break;
		}
	} catch (UDPStream::wrong) { //if the amount of packet data not corresponding to the amount of data that we are trying to read
		std::cout << "--> ALERT: Wrongly serialized data received!" << std::endl;
	} catch (UDPStream::empty) {} //if the package is empty or have not received anything
	//if (clock() > m_aliveCounter + MS_ALIVE_DELAY+1000) std::cout << "Server closed. Disconecting..." << std::endl, Exit();
}

void AppClient::Update(void) {
	ProcessMsgs();
	if (m_currentScreen) { // If current screen exists
		switch (m_currentScreen->currentState) { // Check for the state of the screen
			case ScreenState::RUNNING:
			inputManager.update();	// Update the input manager instance
			m_currentScreen->Update(); // Update the current screen if running
			break;
			case ScreenState::EXIT:
			Exit(); // Call exit function to end the execution
			break;
			case ScreenState::SLEEP: default: break;
		}
	} else Exit(); // Call exit function if screen doesn't exist
}

void AppClient::Draw(void) const {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer
	if (m_currentScreen && m_currentScreen->currentState == ScreenState::RUNNING) { // If screen object exists and its state is running
		m_currentScreen->Draw(); // Then call the draw method of the screen
	}
}

void AppClient::Run(void) {
	Init(); // Call the init everything function
	FPSLimiter fpsLimiter; // Spawn the main instance of the timing limiter
	fpsLimiter.setTargetFPS(TARGET_FPS); // Set the frames per second we whish to have, ideally 60-120

	while (m_isRunning) { // While game is running
		fpsLimiter.begin();					// Init FPS counter
		Update();							// Main update function
		if (!m_isRunning) break;			// Break main game loop if running attribute set to false
		Draw();								// Main draw function
		fps = fpsLimiter.fps;				// Get the current fps of the class instance
		deltaTime = fpsLimiter.deltaTime;	// Get the current fps of the class instance
		fpsLimiter.end();					// Calculate and restore FPS
	}
}

void AppClient::Exit(void) {
	mainSocket << UDPStream::packet << EXIT << serverAddress;
	m_currentScreen->OnExit(); // Call the leaving method of the current screen
	if (m_screenList) {
		m_screenList->Destroy();
		m_screenList.reset();
	}
	SDL_DestroyRenderer(renderer);
	TTF_CloseFont(font);
	DestroySDL();
	m_isRunning = false; // Execution ends
}