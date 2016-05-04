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
	tempCharacter = &gameApp->gameObjectManager.gameObjectList["character_slycooper"];
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
	m_camera.SetPosition({ 0,3,5 });
	m_camera.SetTarget(tempCharacter->transform.position);
}

void PlaygroundScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		gameApp->OnSDLEvent(evnt);
		if (evnt.type == SDL_WINDOWEVENT) {
			switch (evnt.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
				glViewport(0, 0, gameApp->screenWidth, gameApp->screenHeight); // Set the OpenGL viewport to window dimensions
				m_camera.Update();
				break;
			}
		}
	}
	/*
	if (gameApp->inputManager.isKeyDown(SDLK_w)) m_camera.ProcessKeyboard(FORWARD, gameApp->deltaTime);
	if (gameApp->inputManager.isKeyDown(SDLK_a)) m_camera.ProcessKeyboard(LEFT, gameApp->deltaTime);
	if (gameApp->inputManager.isKeyDown(SDLK_s)) m_camera.ProcessKeyboard(BACKWARD, gameApp->deltaTime);
	if (gameApp->inputManager.isKeyDown(SDLK_d)) m_camera.ProcessKeyboard(RIGHT, gameApp->deltaTime);

	static GLfloat lastX = gameApp->inputManager.m_mouseCoords.x;
	static GLfloat lastY = gameApp->inputManager.m_mouseCoords.y;
	GLfloat xoffset = gameApp->inputManager.m_mouseCoords.x - lastX;
	GLfloat yoffset = lastY - gameApp->inputManager.m_mouseCoords.y;
	m_camera.ProcessMouseMovement(xoffset, yoffset);
	lastX = gameApp->inputManager.m_mouseCoords.x;
	lastY = gameApp->inputManager.m_mouseCoords.y;

	m_camera.ProcessMouseScroll(gameApp->inputManager.zoom*0.1f);*/
}

void PlaygroundScreen::Draw() {
	m_shaderProgram.bind();

	// Send camera matrix to shader (projection + view)
	glUniformMatrix4fv(m_shaderProgram.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(m_camera.PVMatrix));

	/*auto &entityHashList = gameApp->gameObjectManager.gameObjectList;
	auto gameObject = entityHash.second;*/

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tempCharacter->texture.id);
	glUniform1i(m_shaderProgram.getUniformLocation("texture_diffuse"), 0);

	glm::mat4 model;
	model = glm::scale(model, tempCharacter->transform.scale);
	model = glm::translate(model, tempCharacter->transform.position);
	glUniformMatrix4fv(m_shaderProgram.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(tempCharacter->mesh.vao);
	glDrawElements(GL_TRIANGLES, tempCharacter->mesh.elements, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	m_shaderProgram.unbind();

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Reset regular alpha blending
}

int PlaygroundScreen::GetNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int PlaygroundScreen::GetPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}