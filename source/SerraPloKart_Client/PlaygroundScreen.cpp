#include "PlaygroundScreen.h"
#include <SerraPloEngine/IAppCLient.h>
#include <SerraPloEngine/ResourceManager.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define FIXED_ASPECT_RATIO 16 / 10

PlaygroundScreen::PlaygroundScreen() {}

PlaygroundScreen::~PlaygroundScreen() {}

void PlaygroundScreen::Build() {
	// Initialize camera with viewport dimensions
	int nw = (gameApp->screenHeight * FIXED_ASPECT_RATIO);
	m_camera.Resize(nw + (gameApp->screenWidth - nw) / 2, gameApp->screenHeight);

	//Initialize texture shaders
	m_mainProgram.LoadShaders(LoadAsset("shaders/main.vert"), LoadAsset("shaders/main.frag"));
	//Initialize light shaders
	m_lightProgram.LoadShaders(LoadAsset("shaders/light.vert"), LoadAsset("shaders/light.frag"));

	// Load the player model
	m_player = &gameApp->gameObjectManager.Find("car_base");
	for (int i = 0; i < 4; i++) m_playerwheels[i] = &gameApp->gameObjectManager.Find("car_wheel");
	// Add the gameobjects needed in this scene
	m_renderer.Add(m_player);
	for (int i = 0; i < 4; i++) m_renderer.Add(m_playerwheels[i]);
	m_renderer.Add(&gameApp->gameObjectManager.Find("character_bb8"));
	m_renderer.Add(&gameApp->gameObjectManager.Find("object_skybox"));
	m_renderer.Add(&gameApp->gameObjectManager.Find("object_circuit"));
	m_renderer.Add(&gameApp->gameObjectManager.Find("character_slycooper"));

	m_carPhy.AddTransform(&m_player->transform);
}

void PlaygroundScreen::Destroy() {

}

void PlaygroundScreen::OnEntry() {
	//SDL_ShowCursor(0);

	// LIGHTNING
	// Init directional light
	m_dirLight.direction = { -0.2f, -1.0f, -0.3f };
	m_dirLight.ambient = { 0.3f, 0.3f, 0.3f };
	m_dirLight.diffuse = { 0.9f, 0.9f, 0.7f };
	m_dirLight.specular = { 0.5f, 0.5f, 0.5f };
	m_renderer.Add(&m_dirLight);

	// Init point lights
	m_pointLights[0].position = { 10, 0, 0 };
	m_pointLights[0].ambient = { 0.5f, 0.0f, 0.5f };
	m_pointLights[0].diffuse = { 1.0f, 0.0f, 1.0f };
	m_pointLights[0].specular = { 1.0f, 1.0f, 1.0f };
	m_pointLights[0].constant = 1.0f;
	m_pointLights[0].linear = 0.09f;
	m_pointLights[0].quadratic = 0.032f;
	m_renderer.Add(&m_pointLights[0]);

	// Init spot lights
	m_spotLights[0].position = { -10,5,0 };
	m_spotLights[0].direction = { 0, -1, 0 };
	m_spotLights[0].ambient = { 0.0f, 0.5f, 0.0f };
	m_spotLights[0].diffuse = { 0.0f, 1.0f, 0.0f };
	m_spotLights[0].specular = { 1.0f, 1.0f, 1.0f };
	m_spotLights[0].constant = 1.0f;
	m_spotLights[0].linear = 0.09f;
	m_spotLights[0].quadratic = 0.032f;
	m_spotLights[0].cutOff = glm::cos(glm::radians(40.0f));
	m_spotLights[0].outerCutOff = glm::cos(glm::radians(45.0f));
	m_renderer.Add(&m_spotLights[0]);

	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
}

void PlaygroundScreen::OnExit() {
	
}

void PlaygroundScreen::Update() {
	checkInput();

	bool temp[5];
	for (int i = 0; i < 5; i++) temp[i] = false;
	if (gameApp->inputManager.isKeyDown(SDLK_w)) temp[0] = true;
	if (gameApp->inputManager.isKeyDown(SDLK_a)) temp[2] = true;
	if (gameApp->inputManager.isKeyDown(SDLK_s)) temp[1] = true;
	if (gameApp->inputManager.isKeyDown(SDLK_d)) temp[3] = true;

	m_carPhy.Update(temp, gameApp->deltaTime);

	m_playerwheels[0]->transform.position = m_player->transform.position + m_carPhy.front*3.0f;
	m_playerwheels[0]->transform.rotation = m_player->transform.rotation - glm::vec3(0.0f,(m_carPhy.steerAngle*180.0f)/M_PI,0.0f);

	m_camera.Translate(m_player->transform.position - (m_carPhy.front*15.0f) + glm::vec3(0.0f, 5.0f, 0.0f));
	m_camera.SetTarget(glm::vec3{ 0,2,0 } +m_player->transform.position);
}

void PlaygroundScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		gameApp->OnSDLEvent(evnt);
		if (evnt.type == SDL_WINDOWEVENT) {
			switch (evnt.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
				SDL_GetWindowSize(gameApp->window.SDLWindow, &gameApp->screenWidth, &gameApp->screenHeight);
				glViewport(0, 0, gameApp->screenWidth, gameApp->screenHeight); // Set the OpenGL viewport to window dimensions
				int nw = (gameApp->screenHeight * FIXED_ASPECT_RATIO);
				m_camera.Resize(nw + (gameApp->screenWidth - nw) / 2, gameApp->screenHeight);
				break;
			}
		}
	}
}

void PlaygroundScreen::Draw() {
	m_mainProgram.bind();
		m_renderer.DrawObjects(m_mainProgram, m_camera);
	m_mainProgram.unbind();

#if LIGHT_DEBUG_MODE
	m_lightProgram.bind();
		m_renderer.DrawLights(m_lightProgram, m_camera);
	m_lightProgram.unbind();
#endif
}

int PlaygroundScreen::GetNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int PlaygroundScreen::GetPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}