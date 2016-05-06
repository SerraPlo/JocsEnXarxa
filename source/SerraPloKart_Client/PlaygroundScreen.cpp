#include "PlaygroundScreen.h"
#include <SerraPloEngine/IAppCLient.h>
#include <SerraPloEngine/ResourceManager.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

PlaygroundScreen::PlaygroundScreen() {}

PlaygroundScreen::~PlaygroundScreen() {}

void PlaygroundScreen::Build() {
	m_camera.Init(gameApp->screenWidth, gameApp->screenHeight); // Initialize camera with viewport dimensions

	m_player = &gameApp->gameObjectManager.gameObjectList["character_slycooper"]; // Load the player model
	// Add the gameobjects needed in this scene
	m_renderer.Add(m_player);
	m_renderer.Add(&gameApp->gameObjectManager.gameObjectList["character_bb8"]);
	m_renderer.Add(&gameApp->gameObjectManager.gameObjectList["character_seahorse"]);
	m_renderer.Add(&gameApp->gameObjectManager.gameObjectList["object_floor"]);

	// Init directional light
	m_dirLight.direction = { -0.2f, -1.0f, -0.3f };
	m_dirLight.ambient = { 0.05f, 0.05f, 0.05f };
	m_dirLight.diffuse = { 0.4f, 0.4f, 0.4f };
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
	m_spotLights[0].position = { 10, 0, 0 };
	m_spotLights[0].ambient = { 0.5f, 0.0f, 0.5f };
	m_spotLights[0].diffuse = { 1.0f, 0.0f, 1.0f };
	m_spotLights[0].specular = { 1.0f, 1.0f, 1.0f };
	m_spotLights[0].constant = 1.0f;
	m_spotLights[0].linear = 0.09f;
	m_spotLights[0].quadratic = 0.032f;
	m_spotLights[0].cutOff = 0.09f;
	m_spotLights[0].outerCutOff = 0.032f;
	m_renderer.Add(&m_spotLights[0]);
}

void PlaygroundScreen::Destroy() {

}

void PlaygroundScreen::OnEntry() {
	//Initialize texture shaders
	m_mainProgram.compileShaders(LoadAsset("shaders/main.vert"), LoadAsset("shaders/main.frag"));
	m_mainProgram.linkShaders();

	//Initialize light shaders
	m_lightProgram.compileShaders(LoadAsset("shaders/light.vert"), LoadAsset("shaders/light.frag"));
	m_lightProgram.linkShaders();

	//SDL_ShowCursor(0);
	//LIGHTNING
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
}

void PlaygroundScreen::OnExit() {
	
}

void PlaygroundScreen::Update() {
	checkInput();

	float direction = 0.0f;
	float rotation = 0.0f;
	if (gameApp->inputManager.isKeyDown(SDLK_w)) direction = 1.0f;
	if (gameApp->inputManager.isKeyDown(SDLK_a)) rotation = 1.0;
	if (gameApp->inputManager.isKeyDown(SDLK_s)) direction = -1.0f;
	if (gameApp->inputManager.isKeyDown(SDLK_d)) rotation = -1.0f;

	m_player->transform.rotation += glm::vec3(0.0f, rotation*gameApp->deltaTime*100.0f, 0.0f);
	glm::vec3 frontPlayer;
	frontPlayer = glm::vec3(sin((m_player->transform.rotation.y*3.14159) / 180), 0.0f, cos((m_player->transform.rotation.y*3.14159) / 180));
	frontPlayer = glm::normalize(frontPlayer);

	m_player->transform.position += direction*frontPlayer*gameApp->deltaTime * 10.0f;
	m_camera.Translate(m_player->transform.position - (frontPlayer*15.0f) + glm::vec3(0.0f, 5.0f, 0.0f));
	m_camera.SetTarget(glm::vec3{ 0,2,0 } +m_player->transform.position);
}

void PlaygroundScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		gameApp->OnSDLEvent(evnt);
		if (evnt.type == SDL_WINDOWEVENT) {
			switch (evnt.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
				glViewport(0, 0, gameApp->screenWidth, gameApp->screenHeight); // Set the OpenGL viewport to window dimensions
				//m_camera.Update();
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