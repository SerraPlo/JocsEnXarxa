#include "GameplayScreen.h"
#include <SDL2/SDL.h>
#include <SerraPloEngine//IMainGame.h>
#include <SerraPloEngine/ResourceManager.h>
#include <ctime>

void GameplayScreen::checkInput() const {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) m_game->onSDLEvent(evnt);
}

GameplayScreen::GameplayScreen(SerraPlo::GLWindow* window) :
	m_window(window), m_mouseLight(nullptr) {}

GameplayScreen::~GameplayScreen() {
	delete m_mouseLight;
}

void GameplayScreen::build() {

}

void GameplayScreen::destroy() {

}

void GameplayScreen::onEntry() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_spriteBatch.init(); //Initialize spritebatch
	m_debugRenderer.init();

	//Initialize texture shaders
	m_textureProgram.compileShaders(SerraPlo::ResourceManager::LoadAsset("shaders/textureShading.vert"), SerraPlo::ResourceManager::LoadAsset("shaders/textureShading.frag"));
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
	//Initialize light shaders
	m_lightProgram.compileShaders(SerraPlo::ResourceManager::LoadAsset("shaders/lightShading.vert"), SerraPlo::ResourceManager::LoadAsset("shaders/lightShading.frag"));
	m_lightProgram.addAttribute("vertexPosition");
	m_lightProgram.addAttribute("vertexColor");
	m_lightProgram.addAttribute("vertexUV");
	m_lightProgram.linkShaders();

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(32.0f);

	//Render light
	m_mouseLight = new SerraPlo::Light();
	m_mouseLight->color = SerraPlo::ColorRGBA8{50, 50, 255, 150};
	m_mouseLight->position = m_camera.convertScreenToWorld(m_game->getInputManager().getMouseCoords());
	m_mouseLight->size = 35.0f;
}

void GameplayScreen::onExit() {
	m_debugRenderer.dispose();
}

void GameplayScreen::update() {
	m_camera.update();
	m_camera.setPosition({0,0});
	checkInput();

	m_mouseLight->position = m_camera.convertScreenToWorld(m_game->getInputManager().getMouseCoords());
}

void GameplayScreen::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);

	m_textureProgram.bind();

		auto textureUniform = m_textureProgram.getUniformLocation("mySampler");
		glUniform1i(textureUniform, 0);

		auto projectionMatrix = m_camera.getCameraMatrix();
		auto projectionUniform = m_textureProgram.getUniformLocation("P");
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

		//push batches to draw
		m_spriteBatch.begin();
			const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
			static int textureID = SerraPlo::ResourceManager::GetTexture("images/objects/wepicon-axe.png").id;
			for (int i = 0; i < 1000; ++i) {
				glm::vec4 destRect(((rand() % 100) - 50)+ sin(float(clock()*0.001f))*5.0f, ((rand() % 100) - 50) + cos(float(clock()*0.001f))*5.0f, 1, 1);
				m_spriteBatch.pushBatch(destRect, uvRect, textureID, 0.0f, SerraPlo::ColorRGBA8 { 255, 255, 255, 255 });
			}
		m_spriteBatch.end();
		m_spriteBatch.renderBatches();

	m_textureProgram.unbind();

	m_lightProgram.bind();

		projectionUniform = m_lightProgram.getUniformLocation("P");
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Additive blending

		m_spriteBatch.begin();
			m_mouseLight->draw(m_spriteBatch);
		m_spriteBatch.end();
		m_spriteBatch.renderBatches();

	m_lightProgram.unbind();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Reset regular alpha blending
}

int GameplayScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}