#include "PlaygroundScreen.h"
#include <SerraPloEngine/IAppCLient.h>
#include <SerraPloEngine/ResourceManager.h>
#include <ctime>
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>

void PlaygroundScreen::checkInput() const {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) gameApp->OnSDLEvent(evnt);
}

PlaygroundScreen::PlaygroundScreen() : m_mouseLight(nullptr), m_particleBatch(nullptr) {}

PlaygroundScreen::~PlaygroundScreen() {
	delete m_mouseLight;
	//delete m_particleBatch;
}

void PlaygroundScreen::Build() {
	m_particleBatch = new ParticleBatch2D;
	m_particleBatch->init(1000, 0.05f, ResourceManager::GetTexture("images/tank-hero/exp1.png").id,
		[](Particle2D& particle, float deltaTime) {
		particle.position += particle.velocity*deltaTime;
		particle.color.a = static_cast<GLubyte>(particle.life * 255.0f);
	});
	m_particleEngine.addParticleBatch(m_particleBatch);
}

void PlaygroundScreen::Destroy() {
	
}

void PlaygroundScreen::OnEntry() {
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

	SDL_ShowCursor(0);
	m_camera.init(gameApp->window.getScreenWidth(), gameApp->window.getScreenHeight());
	m_camera.setScale(32.0f);

	//Render light
	m_mouseLight = new SerraPlo::Light2D();
	m_mouseLight->color = SerraPlo::ColorRGBA8{50, 50, 255, 150};
	m_mouseLight->position = &m_camera.mouseScreenCoords;
	m_mouseLight->size = 50.0f;
}

void PlaygroundScreen::OnExit() {
	m_debugRenderer.dispose();
}

void PlaygroundScreen::Update() {
	m_camera.update(gameApp->inputManager.m_mouseCoords);
	m_camera.setPosition({0,0});
	checkInput();

	for (auto i = 0; i < 50; i++)
		m_particleBatch->addParticle({ sin(float(clock() / 300.0f))*10.0f , cos(float(clock() / 300.0f))*10.0f }, glm::rotate(glm::vec2(2.0f, 0.0f), float((rand() % 3600) / 10.0f)), 5.0f, ColorRGBA8{ GLubyte(rand() % 255), GLubyte(rand() % 255),GLubyte(rand() % 255), 255 });
	m_particleEngine.update(1.0f);
}

void PlaygroundScreen::Draw() {
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
			static int textureID = SerraPlo::ResourceManager::GetTexture("images/interface/marijuana.png").id;
			for (int i = 0; i < 500; ++i) {
				glm::vec4 destRect((i%20) - 10 + sin(float(clock()/500.0f))*5.0f, ((i/20) - 10 + cos(float(clock()/500.0f))*5.0f), 1, 1);
				m_spriteBatch.pushBatch(destRect, uvRect, textureID, 0.0f, SerraPlo::ColorRGBA8 { 255, 255, 255, 255 });
			}
			glm::vec4 destRect(m_camera.mouseScreenCoords.x, m_camera.mouseScreenCoords.y, 1, 1);
			m_spriteBatch.pushBatch(destRect, uvRect, textureID, 0.0f, SerraPlo::ColorRGBA8 { 255, 255, 255, 255 });
			m_particleEngine.draw(m_spriteBatch);
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

int PlaygroundScreen::GetNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int PlaygroundScreen::GetPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}