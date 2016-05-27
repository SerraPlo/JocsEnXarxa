#include <SerraPloEngine/PathLoader.h>
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
	font = TTF_OpenFont(GetPathToAsset("fonts/ARIAL.TTF").c_str(), FONT_SIZE);
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	InitOpenGL(); // Initialize OpenGL systems after GLWindow creation

	// Add the screens of the derived app into the list
	m_menuScreen = std::make_unique<MenuScreen>();
	m_screenList->AddScreen(m_menuScreen.get(), SCREEN_MENU);
	m_loginScreen = std::make_unique<LoginScreen>();
	m_screenList->AddScreen(m_loginScreen.get(), SCREEN_LOGIN);
	m_gameplayScreen = std::make_unique<MultiplayerScreen>();
	m_screenList->AddScreen(m_gameplayScreen.get(), SCREEN_MULTIPLAYER);
	m_currentScreen = m_screenList->SetScreen(SCREEN_MENU);

	m_aliveCounter = float(clock());
}

void AppClient::LoadAssets(void) {
	clock_t bench = clock();
	std::cout << "==== LOAD ASSETS BEGIN ====" << std::endl;
	assetManager.LoadMesh("mesh_kart_default", "models/kart/kart_default.obj");
	assetManager.LoadMesh("mesh_wheel_front", "models/kart/wheel_front.obj");
	assetManager.LoadMesh("mesh_wheel_back", "models/kart/wheel_back.obj");
	assetManager.LoadMesh("mesh_colisions", "models/circuit_col/colisions.dae");
	assetManager.LoadMesh("mesh_skybox", "models/skybox/skybox.obj");
	std::cout << "Meshes loaded (" << ((clock() - bench) / 1000.0f) << " seconds)" << std::endl;
	bench = clock();

	assetManager.LoadMaterial("material_kart_default", "models/kart/textures/body_dummy_alb.png");
	assetManager.LoadMaterial("material_red", "images/plch/red.jpg");
	assetManager.LoadMaterial("material_green", "images/plch/green.jpg");
	assetManager.LoadMaterial("material_blue", "images/plch/blue.jpg");
	assetManager.LoadMaterial("material_skybox", "models/skybox/skybox_diffuse.jpg");
	std::cout << "Materials loaded (" << ((clock() - bench) / 1000.0f) << " seconds)" << std::endl;
	bench = clock();

	//assetManager.Load("mesh_skybox", "material_skybox", "models/skybox/skybox.obj");
	assetManager.Load("mesh_circuit", "material_circuit", "models/circuit/circuit.dae");
	std::cout << "Circuit loaded (" << ((clock() - bench) / 1000.0f) << " seconds)" << std::endl;
	std::cout << "==== LOAD ASSETS END ====" << std::endl;
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
			case MSG_BEGIN: {
				std::cout << "Server accepted entry. Game begins." << std::endl; 
				//ChangeScreen(SCREEN_MULTIPLAYER);
			} break;
			case MSG_EXIT: {
				std::cout << "Server closed. Disconecting..." << std::endl;
				Exit();
			} break;
			case MSG_ALIVE: {
				std::cout << "Server is alive" << std::endl;
				m_aliveCounter = float(clock());
			} break;
			case MSG_UPDATE: {
				Enemy a;
				mainSocket >> a.nick >> a.transform.position.x >> a.transform.position.z >> a.transform.rotation.y;
				std::cout << a.nick << std::endl;
				if (a.nick == nick); ///mirar trampes TODO
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
	//if (m_currentScreen) { // If current screen exists
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
	//} else Exit(); // Call exit function if screen doesn't exist
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

	const int BENCH_DELAY = 200;
	int benchCounter = 0;
	clock_t benchUpdate = 0;
	clock_t benchDraw = 0;
	clock_t bench;
	while (m_isRunning) { // While game is running
		fpsLimiter.begin();					// Init FPS counter
		bench = clock();
		Update();							// Main update function
		benchUpdate += clock() - bench;
		if (!m_isRunning) break;			// Break main game loop if running attribute set to false
		bench = clock();
		Draw();								// Main draw function
		benchDraw += clock() - bench;
		fpsLimiter.end();					// Calculate and restore FPS
		fps = fpsLimiter.fps;				// Get the current fps of the class instance
		deltaTime = fpsLimiter.deltaTime;	// Get the current fps of the class instance
		++benchCounter;
		if (benchCounter > BENCH_DELAY)
			std::cout << "------------------------------------------" << std::endl,
			std::cout << "Update call:\t" << float(benchUpdate / BENCH_DELAY) << " ms" << std::endl,
			std::cout << "Draw call:\t" << float(benchDraw / BENCH_DELAY) << " ms" << std::endl,
			std::cout << "------------------------------------------" << std::endl,
			benchCounter = benchUpdate = benchDraw = 0;
	}
}

void AppClient::Exit(void) {
	mainSocket << UDPStream::packet << MSG_EXIT << serverAddress;
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