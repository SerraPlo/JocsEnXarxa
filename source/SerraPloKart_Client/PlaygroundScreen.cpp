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
	m_renderer.Add(&tempLight);
}

void PlaygroundScreen::Destroy() {

}

struct Vertex {
	glm::vec3 m_pos;
	glm::vec2 m_tex;

	Vertex() {}

	Vertex(glm::vec3 pos, glm::vec2 tex) {
		m_pos = pos;
		m_tex = tex;
	}
};

void PlaygroundScreen::OnEntry() {
	//Initialize texture shaders
	m_mainProgram.compileShaders(LoadAsset("shaders/light.vert"), LoadAsset("shaders/light.frag"));
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

	glm::vec3 direction;
	if (gameApp->inputManager.isKeyDown(SDLK_w)) direction += glm::vec3(0.0f, 0.0f, 1.0f);
	if (gameApp->inputManager.isKeyDown(SDLK_a)) direction += glm::vec3(1.0f, 0.0f, 0.0f);
	if (gameApp->inputManager.isKeyDown(SDLK_s)) direction += glm::vec3(0.0f, 0.0f, -1.0f);
	if (gameApp->inputManager.isKeyDown(SDLK_d)) direction += glm::vec3(-1.0f, 0.0f, 0.0f);

	m_player->transform.position += direction*gameApp->deltaTime*8.0f;

	m_camera.Translate(glm::vec3{ 0,5,-12 } + m_player->transform.position);
	m_camera.SetTarget(glm::vec3{ 0,2,0 } + m_player->transform.position);
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
		// Send camera matrix to shader (projection + view)
		glUniformMatrix4fv(m_mainProgram.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(m_camera.PVMatrix())); 
		m_renderer.DrawObjects(m_mainProgram);
		m_renderer.DrawLights(m_mainProgram);
	m_mainProgram.unbind();

	/*m_lightProgram.bind();
		glUniformMatrix4fv(m_lightProgram.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(m_camera.PVMatrix()));
		m_renderer.DrawLights(m_lightProgram);
	m_lightProgram.unbind();*/
}

int PlaygroundScreen::GetNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int PlaygroundScreen::GetPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}