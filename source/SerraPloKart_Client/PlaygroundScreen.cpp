#include "PlaygroundScreen.h"
#include <SerraPloEngine/IAppCLient.h>
#include <SerraPloEngine/ResourceManager.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

PlaygroundScreen::PlaygroundScreen() {}

PlaygroundScreen::~PlaygroundScreen() {}

void PlaygroundScreen::Build() {
	m_camera.Init(gameApp->screenWidth, gameApp->screenHeight);
	m_player = &gameApp->gameObjectManager.gameObjectList["character_slycooper"];
	m_sceneObjects.push_back(m_player);
	m_sceneObjects.push_back(&gameApp->gameObjectManager.gameObjectList["character_seahorse"]);
}

void PlaygroundScreen::Destroy() {

}

void PlaygroundScreen::OnEntry() {
	//Initialize texture shaders
	m_shaderProgram.compileShaders(LoadAsset("shaders/main.vert"), LoadAsset("shaders/main.frag"));
	m_shaderProgram.linkShaders();

	//SDL_ShowCursor(0);
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
	m_shaderProgram.bind();

	// Send camera matrix to shader (projection + view)
	glUniformMatrix4fv(m_shaderProgram.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(m_camera.PVMatrix()));

	static float counter = 0.0f;
	for (auto gameObject : m_sceneObjects) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gameObject->texture.id);
		glUniform1i(m_shaderProgram.getUniformLocation("texture_diffuse"), 0);

		glm::mat4 model;
		model = glm::translate(model, gameObject->transform.position);
		model = glm::rotate(model, gameObject->transform.rotation.x, glm::vec3{1,0,0});
		if (gameObject->id == "character_seahorse") model = glm::rotate(model, glm::radians(counter++), glm::vec3{ 0,1,0 });
		model = glm::rotate(model, gameObject->transform.rotation.z, glm::vec3{ 0,0,1 });
		model = glm::scale(model, gameObject->transform.scale);
		glUniformMatrix4fv(m_shaderProgram.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(gameObject->mesh.vao);
		glDrawElements(GL_TRIANGLES, gameObject->mesh.elements, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
	
	m_shaderProgram.unbind();

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Reset regular alpha blending
}

int PlaygroundScreen::GetNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int PlaygroundScreen::GetPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}