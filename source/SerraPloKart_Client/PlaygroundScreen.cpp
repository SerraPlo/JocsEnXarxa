#include "PlaygroundScreen.h"
#include <SerraPloEngine/IAppCLient.h>
#include <SerraPloEngine/ResourceManager.h>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

PlaygroundScreen::PlaygroundScreen() {}

PlaygroundScreen::~PlaygroundScreen() {}

void PlaygroundScreen::Build() {
	m_camera.Init(gameApp->screenWidth, gameApp->screenHeight);
}

void PlaygroundScreen::Destroy() {
	m_textureProgram.dispose();
}

void PlaygroundScreen::OnEntry() {
	//Initialize texture shaders
	m_textureProgram.compileShaders(ResourceManager::LoadAsset("shaders/textureShading.vert"), ResourceManager::LoadAsset("shaders/textureShading.frag"));
	m_textureProgram.linkShaders();

	//SDL_ShowCursor(0);
	m_camera.position = { 0,0,3 };

	m_renderer.Init();

	// Load and create a texture 
	glGenTextures(1, &texture1);
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
	while (SDL_PollEvent(&evnt)) gameApp->OnSDLEvent(evnt);

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

// World space positions of our cubes
static glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

void PlaygroundScreen::Draw() {
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(m_textureProgram.getUniformLocation("texture_diffuse"), 0);

	m_textureProgram.bind();

	glm::mat4 cameraMatrix = m_camera.GetMatrix();	
	GLint cameraUniform = m_textureProgram.getUniformLocation("camera");
	glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, glm::value_ptr(cameraMatrix));

	GLint modelLoc = m_textureProgram.getUniformLocation("model");
	m_renderer.Begin();
	for (GLuint i = 0; i < 10; i++) {
		// Calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		GLfloat angle = 20.0f * i;
		model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		m_renderer.Render();
	}
	m_renderer.End();

	m_textureProgram.unbind();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Reset regular alpha blending
}

int PlaygroundScreen::GetNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int PlaygroundScreen::GetPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}