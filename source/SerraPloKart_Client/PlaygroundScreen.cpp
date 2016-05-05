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
	m_sceneObjects.Add(m_player);
	m_sceneObjects.Add(&gameApp->gameObjectManager.gameObjectList["character_bb8"]);
	m_sceneObjects.Add(&gameApp->gameObjectManager.gameObjectList["character_seahorse"]);
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
	m_mainProgram.compileShaders(LoadAsset("shaders/main.vert"), LoadAsset("shaders/main.frag"));
	m_mainProgram.linkShaders();

	//Initialize light shaders
	m_lightProgram.compileShaders(LoadAsset("shaders/light.vert"), LoadAsset("shaders/light.frag"));
	m_lightProgram.linkShaders();

	//SDL_ShowCursor(0);
	//LIGHTNING
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	// First, set the container's VAO (and VBO)
	
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
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
		m_sceneObjects.Draw(m_mainProgram);
	m_mainProgram.unbind();

	m_lightProgram.bind();
	/*GLint objectColorLoc = m_lightProgram.getUniformLocation("objectColor");
	GLint lightColorLoc = m_lightProgram.getUniformLocation("lightColor");
	GLint lightPosLoc = m_lightProgram.getUniformLocation("lightPos");
	GLint viewPosLoc = m_lightProgram.getUniformLocation("viewPos");
	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0, 0, 0); //light pos
	glUniform3f(viewPosLoc, m_camera.position.x, m_camera.position.y, m_camera.position.z);
	glUniformMatrix4fv(m_lightProgram.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(m_camera.PVMatrix()));
	glBindVertexArray(containerVAO);
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, { 0, 0, 0 });
	model = glm::scale(model, glm::vec3(0.2f));
	glUniformMatrix4fv(m_lightProgram.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);*/

	m_lightProgram.unbind();
}

int PlaygroundScreen::GetNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int PlaygroundScreen::GetPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}