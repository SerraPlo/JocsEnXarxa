#include "PlaygroundScreen.h"
#include <SerraPloEngine/IAppCLient.h>
#include <SerraPloEngine/ResourceManager.h>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

PlaygroundScreen::PlaygroundScreen() {
	
}

PlaygroundScreen::~PlaygroundScreen() {}

void PlaygroundScreen::Build() {
	m_camera.Init(gameApp->screenWidth, gameApp->screenHeight);
	testMesh.Load(ResourceManager::LoadAsset("models/seahorse/seahorse.obj").c_str());
}

void PlaygroundScreen::Destroy() {

}

void PlaygroundScreen::OnEntry() {
	//Initialize texture shaders
	m_mainProgram.compileShaders(ResourceManager::LoadAsset("shaders/textureShading.vert"), ResourceManager::LoadAsset("shaders/textureShading.frag"));
	m_mainProgram.linkShaders();

	//SDL_ShowCursor(0);
	m_camera.SetPosition({ 0,0,3 });

	//m_renderer.Init();

	// Load and create a texture 
	/*glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
											// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(ResourceManager::LoadAsset("images/container.jpg").c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.*/
}

void PlaygroundScreen::OnExit() {
	
}

void PlaygroundScreen::Update() {
	checkInput();
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

	m_camera.ProcessMouseScroll(gameApp->inputManager.zoom*0.1f);
}

void PlaygroundScreen::Draw() {
	m_mainProgram.bind();

	GLint cameraUniform = m_mainProgram.getUniformLocation("camera");
	glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, glm::value_ptr(m_camera.PVMatrix));

	GLint modelLoc = m_mainProgram.getUniformLocation("model");
	glm::mat4 model;
	model = glm::scale(model, { 0.05,0.05,0.05 });
	model = glm::translate(model, { 0,0,0 });
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(testMesh.vao);
	glDrawElements(GL_TRIANGLES, testMesh.elements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	m_mainProgram.unbind();

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Reset regular alpha blending
}

int PlaygroundScreen::GetNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int PlaygroundScreen::GetPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}