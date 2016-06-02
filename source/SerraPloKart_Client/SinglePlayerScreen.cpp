#include <SerraPloEngine/PathLoader.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "SinglePlayerScreen.h"
#include "AppClient.h"

#define FIXED_ASPECT_RATIO 16 / 9

void SinglePlayerScreen::Build(void) {
	m_app = dynamic_cast<AppClient*>(gameApp);

	// Initialize camera with viewport dimensions
	int nw = (m_app->screenHeight * FIXED_ASPECT_RATIO);
	m_camera.Resize(nw + (m_app->screenWidth - nw) / 2, m_app->screenHeight);
	m_minimapCamera.Resize(nw + (m_app->screenWidth - nw) / 2, m_app->screenHeight);

	//Initialize main shaders
	m_mainProgram.LoadShaders("shaders/main.vert", "shaders/main.frag");
	m_screenProgram.LoadShaders("shaders/screen.vert", "shaders/screen.frag");
	//Initialize debug shaders
	m_debugProgram.LoadShaders("shaders/debug.vert", "shaders/debug.frag");

	// Init IA Path with 12 points for steering path following
	m_aiPath = {
		{ 40, 108 },
		{ 10, 80 },
		{ -3, 7 },
		{ -40, -43 },
		{ -35, -70 },
		{ 10, -108 },
		{ 40, -100 },
		{ 165, 10 },
		{ 290, 20 },
		{ 315, 50 },
		{ 308, 90 },
		{ 280, 108 }
	}; m_aiPhysics.AddPath(&m_aiPath);

	// Init game physics
	m_carPhysics.AddTransform(&m_player.body.transform);
	m_aiPhysics.AddAICar(&m_aiEnemies[0].body.transform, 0.85f, 200.0f * 60.0f);
	m_aiPhysics.AddAICar(&m_aiEnemies[1].body.transform, 0.9f, 200.0f * 60.0f);
	m_aiPhysics.AddAICar(&m_aiEnemies[2].body.transform, 1.2f, 200.0f * 60.0f);
	m_aiPhysics.AddAICar(&m_aiEnemies[3].body.transform, 1.4f, 200.0f * 60.0f);
	
	m_renderer.InitFramebuffer(m_app->screenWidth, m_app->screenHeight);

	// Init PowerUps
	//powerUp.carPositions.push_back(&m_player.body.transform.position);
	//for (int i = 0; i < MAX_AI_ENEMIES; ++i) powerUp.carPositions.push_back(&m_aiEnemies[i].body.transform.position);
}

void SinglePlayerScreen::Destroy(void) {
	for (auto pu : powerUpList) delete pu;
}

void SinglePlayerScreen::OnEntry(void) {
	//SDL_ShowCursor(0);

	// Load player base kart model
	m_player.body.transform.position = { 180, 0, 115 };
	m_player.body.transform.rotation = { 0, -90, 0 };
	m_player.body.meshRef = &m_app->assetManager.FindMesh("mesh_kart_default");
	m_player.body.materialRef = &m_app->assetManager.FindMaterial("material_kart_default");
	m_player.body.materialRef->materialData[0].shininess = 50;
	m_player.body.materialRef->materialData[0].specular = { 1,1,1 };
	m_renderer.AddObject(&m_player.body);

	/// TODO: set emissive color !! material parameters!
	skybox.transform.position = { 0, -100, 0 };
	skybox.transform.scale = { 3, 3, 3 };
	skybox.meshRef = &m_app->assetManager.FindMesh("mesh_skybox");
	skybox.materialRef = &m_app->assetManager.FindMaterial("material_skybox");
	m_renderer.AddObject(&skybox);

	circuit.transform.scale = { 1, 1, 0.2 };
	circuit.meshRef = &m_app->assetManager.FindMesh("mesh_circuit");
	circuit.materialRef = &m_app->assetManager.FindMaterial("material_circuit");
	m_renderer.AddObject(&circuit);

	debugCollisions.color = { 0,0.5, 0 };
	debugCollisions.meshRef = &m_app->assetManager.FindMesh("mesh_debug_collisions");
	m_renderer.AddObject(&debugCollisions);

	// LIGHTNING
	// Init directional light
	m_dirLight.direction = { -0.2f, -1.0f, -0.3f };
	m_dirLight.ambient = { 0.05f, 0.05f, 0.15f };
	m_dirLight.diffuse = { 0.1f, 0.1f, 0.2f };
	m_dirLight.specular = { 0.1f, 0.1f, 0.1f };
	m_renderer.AddLight(&m_dirLight);

	// Init point lights
	m_pointLights[0].position = { 150, 0, 100 };
	m_pointLights[1].position = { 150, 0, 120 };
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i) {
		m_pointLights[i].ambient = { 0.3f, 1.0f, 0.3f };
		m_pointLights[i].diffuse = { 0.5f, 1.0f, 0.5f };
		m_pointLights[i].specular = { 1.0f, 1.0f, 1.0f };
		m_pointLights[i].constant = 1.0f;
		m_pointLights[i].linear = 0.09f;
		m_pointLights[i].quadratic = 0.032f;
		m_renderer.AddLight(&m_pointLights[i]);
	}

	// Init spot lights
	m_spotLights[0].position = { 40, 8, 108 };
	m_spotLights[1].position = { 10, 8, 80 };
	m_spotLights[2].position = { -3, 8, 7 };
	m_spotLights[3].position = { -40, 8, -43 };
	m_spotLights[4].position = { -35, 8, -70 };
	m_spotLights[5].position = { 10, 8, -108 };
	m_spotLights[6].position = { 40, 8, -100 };
	m_spotLights[7].position = { 165, 8, 10 };
	m_spotLights[8].position = { 290, 8, 20 };
	m_spotLights[9].position = { 315, 8, 50 };
	m_spotLights[10].position = { 308, 8, 90 };
	m_spotLights[11].position = { 280, 8, 108 };
	for (int i = 0; i < MAX_STATIC_SPOT_LIGHTS; ++i) {
		m_spotLights[i].direction = { 0, -1, 0 };
		m_spotLights[i].ambient = { 1.0f, 1.0f, 1.0f };
		m_spotLights[i].diffuse = { 1.0f, 1.0f, 0.7f };
		m_spotLights[i].specular = { 1.0f, 1.0f, 1.0f };
		m_spotLights[i].constant = 1.0f;
		m_spotLights[i].linear = 0.09f;
		m_spotLights[i].quadratic = 0.032f;
		m_spotLights[i].cutOff = glm::cos(glm::radians(50.0f));
		m_spotLights[i].outerCutOff = glm::cos(glm::radians(60.0f));
		m_renderer.AddLight(&m_spotLights[i]);
	}

	m_player.light.position = m_player.body.transform.position;
	m_player.light.direction = { -1, 0, 0 };
	m_player.light.ambient = { 1.0f, 1.0f, 1.0f };
	m_player.light.diffuse = { 1.0f, 1.0f, 0.5f };
	m_player.light.specular = { 1.0f, 1.0f, 1.0f };
	m_player.light.constant = 1.0f;
	m_player.light.linear = 0.027f;
	m_player.light.quadratic = 0.0028f;
	m_player.light.cutOff = glm::cos(glm::radians(30.0f));
	m_player.light.outerCutOff = glm::cos(glm::radians(40.0f));
	m_renderer.AddLight(&m_player.light, false);

	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0

	// Init IA enemies
	m_aiEnemies[0].body.transform.position = { 180, 0, 115 };
	m_aiEnemies[1].body.transform.position = { 180, 0, 105 };
	m_aiEnemies[2].body.transform.position = { 200, 0, 115 };
	m_aiEnemies[3].body.transform.position = { 200, 0, 105 };
	for (int i = 0; i < MAX_AI_ENEMIES; ++i) {
		m_aiEnemies[i].body.meshRef = &m_app->assetManager.FindMesh("mesh_kart_default");
		m_aiEnemies[i].body.materialRef = &m_app->assetManager.FindMaterial("material_kart_0" + std::to_string(i));
		m_aiEnemies[i].body.materialRef->materialData[0].shininess = 50;
		m_aiEnemies[i].body.materialRef->materialData[0].specular = { 1,1,1 };
		m_renderer.AddObject(&m_aiEnemies[i].body);

		m_aiEnemies[i].light.ambient = { 1.0f, 1.0f, 1.0f };
		m_aiEnemies[i].light.diffuse = { 1.0f, 1.0f, 0.5f };
		m_aiEnemies[i].light.specular = { 1.0f, 1.0f, 1.0f };
		m_aiEnemies[i].light.constant = 1.0f;
		m_aiEnemies[i].light.linear = 0.027f;
		m_aiEnemies[i].light.quadratic = 0.0028f;
		m_aiEnemies[i].light.cutOff = glm::cos(glm::radians(30.0f));
		m_aiEnemies[i].light.outerCutOff = glm::cos(glm::radians(40.0f));
		m_renderer.AddLight(&m_aiEnemies[i].light, false);
	}

	// Init Item Box
	itemBox.transform.position = { 140, 0, 110 };
	itemBox.meshRef = &m_app->assetManager.FindMesh("mesh_item_box");
	itemBox.materialRef = &m_app->assetManager.FindMaterial("material_item_box");
	itemBox.materialRef->materialData[0].shininess = 100;
	itemBox.materialRef->materialData[0].specular = { 1.5,1.5,1.5 };
	m_renderer.AddObject(dynamic_cast<GameObject*>(&itemBox));

	// Init power up list
	PowerUp *newPU = new GreenShell;
	newPU->meshRef = &m_app->assetManager.FindMesh("mesh_item_box");
	powerUpList.push_back(newPU);

	// Send light and material attributes to fragment shader
	m_mainProgram.Bind();
	m_renderer.SendStaticLightAttributes(m_mainProgram, m_camera);
	m_renderer.SendMaterialAttributes(m_mainProgram, m_camera);
	m_mainProgram.Unbind();
}

void SinglePlayerScreen::OnExit(void) {
	m_renderer.Clear();
	m_aiPhysics.Reset();
}

void SinglePlayerScreen::Update(void) {
	// Input update
	static int m_inputCounter = 0;
	static input10 m_in2send;
	CheckInput();
	static bool temp[5];
	memset(temp, false, 5); // reset all elements to false
	if (m_app->inputManager.isKeyDown(SDLK_w)) temp[0] = true;
	if (m_app->inputManager.isKeyDown(SDLK_a)) temp[2] = true;
	if (m_app->inputManager.isKeyDown(SDLK_s)) temp[1] = true;
	if (m_app->inputManager.isKeyDown(SDLK_d)) temp[3] = true;
	if (m_app->inputManager.isKeyDown(SDLK_SPACE)) temp[4] = true;

	// Car physics update
	m_carPhysics.Update(temp, gameApp->deltaTime, { 0.0f,0.0f });

	//Update car lights position & direction
	m_player.light.position = m_player.body.transform.position + m_carPhysics.front*2.0f + glm::vec3{ 0,1,0 };
	m_player.light.direction = m_carPhysics.front - glm::vec3{ 0,0.3f,0 };

	// IA update
	m_aiPhysics.Update(gameApp->deltaTime);
	for (int i = 0; i < MAX_AI_ENEMIES; ++i) {
		glm::vec2 direction = glm::normalize(m_aiPhysics.aiCarArray[i].speed);
		m_aiEnemies[i].light.position = m_aiEnemies[i].body.transform.position + glm::vec3{ direction.x, 0, direction.y }*2.0f + glm::vec3{ 0,1,0 };
		m_aiEnemies[i].light.direction = glm::vec3{ direction.x, 0, direction.y } - glm::vec3{ 0,0.3f,0 };
	}

	// Main camera update
	///m_camera.Translate(m_aiEnemies[0].body.transform.position - (m_carPhysics.front*35.0f) + glm::vec3(0.0f, 15.0f, 0.0f));
	///m_camera.SetTarget(glm::vec3{ 0,2,0 } +m_aiEnemies[0].body.transform.position);
	m_camera.Translate(m_player.body.transform.position - (m_carPhysics.front*35.0f) + glm::vec3(0.0f, 15.0f, 0.0f));
	m_camera.SetTarget(glm::vec3{ 0,2,0 } +m_player.body.transform.position);

	// Minimap camera update
	//m_minimapCamera.Translate({ 140, 450, -2 });
	m_minimapCamera.Translate({ 140, 200, -2 });
	m_minimapCamera.SetTarget(glm::vec3{ 0,2,0 } +m_player.body.transform.position);

	// PowerUp spawner update
	if (clock() > itemBox.activeCounter + POWERUP_SPAWN_DELAY) {
		itemBox.enabled = true;
		itemBox.transform.position.y = 2.0f + sin(clock()*0.01f)*30.0f*m_app->deltaTime;
		itemBox.transform.rotation.y = (clock() / 100) % 360;
		for (int i = 0; i < MAX_AI_ENEMIES; ++i) {
			if (glm::length(m_aiEnemies[i].body.transform.position - itemBox.transform.position) < POWERUP_DETECT_DISTANCE) {
				itemBox.activeCounter = clock();
				itemBox.enabled = false;
				m_aiEnemies[i].powerUp = powerUpList[int(rand()%powerUpList.size())];
				break;
			}
		}
		if (itemBox.enabled && glm::length(m_player.body.transform.position - itemBox.transform.position) < POWERUP_DETECT_DISTANCE) {
			itemBox.activeCounter = clock();
			itemBox.enabled = false;
			m_player.powerUp = powerUpList[int(rand() % powerUpList.size())];
		}
	}

	// ESC
	if (m_app->inputManager.isKeyPressed(SDLK_ESCAPE)) m_app->ChangeScreen(SCREEN_MENU);
}

void SinglePlayerScreen::CheckInput(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt)) {
		m_app->OnSDLEvent(evnt);
		if (evnt.type == SDL_WINDOWEVENT) {
			switch (evnt.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					SDL_GetWindowSize(m_app->window.SDLWindow, &m_app->screenWidth, &m_app->screenHeight);
					glViewport(0, 0, m_app->screenWidth, m_app->screenHeight); // Set the OpenGL viewport to window dimensions
					int nw = (m_app->screenHeight * FIXED_ASPECT_RATIO);
					m_camera.Resize(nw + (m_app->screenWidth - nw) / 2, m_app->screenHeight);
					break;
			}
		}
	}
	if (m_app->inputManager.isKeyPressed(SDLK_e)) RendererList::DEBUG_DRAW = !RendererList::DEBUG_DRAW;
	if (m_app->inputManager.isKeyPressed(SDLK_q)) RendererList::WIREFRAME_MODE = !RendererList::WIREFRAME_MODE;
}

void SinglePlayerScreen::Draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	m_renderer.DrawObjects(m_mainProgram, m_camera);
	m_renderer.DrawFramebuffer(m_mainProgram, m_screenProgram, m_minimapCamera);

	if (RendererList::DEBUG_DRAW) m_renderer.DrawDebug(m_debugProgram, m_camera);

	m_app->window.swapBuffer(); // Swap OpenGL buffers if double-buffering is supported
}