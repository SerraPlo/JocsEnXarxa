#include <SerraPloEngine/ResourceManager.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "MultiplayerScreen.h"
#include "AppClient.h"

#define FIXED_ASPECT_RATIO 16 / 9

void MultiplayerScreen::Build(void) {
	m_app = dynamic_cast<AppClient*>(gameApp);

	// Initialize camera with viewport dimensions
	int nw = (m_app->screenHeight * FIXED_ASPECT_RATIO);
	m_camera.Resize(nw + (m_app->screenWidth - nw) / 2, m_app->screenHeight);

	//Initialize texture shaders
	m_mainProgram.LoadShaders(LoadAsset("shaders/main.vert"), LoadAsset("shaders/main.frag"));
	//Initialize light shaders
	m_lightProgram.LoadShaders(LoadAsset("shaders/light.vert"), LoadAsset("shaders/light.frag"));
}

void MultiplayerScreen::Destroy(void) {
	
}

void MultiplayerScreen::OnEntry(void) {
	//SDL_ShowCursor(0);

	// Load the player model
	m_player = &m_app->gameObjectManager.Find("car_base");

	// Add the gameobjects needed in this scene
	m_renderer.Add(m_player);
	for (int i = 0; i < 4; i++) {
		m_playerwheels[i] = m_app->gameObjectManager.Find("car_wheel");
		m_renderer.Add(&m_playerwheels[i]);
	}
	m_renderer.Add(&m_app->gameObjectManager.Find("object_skybox"));
	m_renderer.Add(&m_app->gameObjectManager.Find("object_circuit"));
	m_renderer.Add(&m_app->gameObjectManager.Find("character_slycooper"));

	m_renderer.AddDebug(&m_app->gameObjectManager.Find("debug_colisions"));

	m_carPhy.AddTransform(&m_player->transform);

	/*for (int i = 0; i < 116; i++) markersCol[i] = gameApp->gameObjectManager.Find("cube");
	for (int i = 0; i < 116; i++) m_renderer.Add(&markersCol[i]);
	for (int i = 0; i < 116; i++) {
	markersCol[i].transform.position = glm::vec3(m_carPhy.collisions.boxs[(int)(i/4)].v[i % 4].x, 10.0f, m_carPhy.collisions.boxs[(int)(i / 4)].v[i % 4].y);
	std::cout << (int)i/4<<": "<< markersCol[i].transform.position.x << ", " << markersCol[i].transform.position.z << std::endl;
	}*/
	
	// Set nick to text plane
	m_textNick.message = m_app->nick;
	m_textNick.scale = {2,1,2};

	// LIGHTNING
	// Init directional light
	m_dirLight.direction = { -0.2f, -1.0f, -0.3f };
	m_dirLight.ambient = { 0.7f, 0.7f, 0.7f };
	m_dirLight.diffuse = { 1.0f, 1.0f, 0.7f };
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

	m_mainProgram.bind();
	m_renderer.SendLightAttributes(m_mainProgram, m_camera);
	m_renderer.SendMaterialAttributes(m_mainProgram, m_camera);
	m_mainProgram.unbind();
}

void MultiplayerScreen::OnExit(void) {
	m_renderer.Clear();
}

void MultiplayerScreen::Update(void) {
	CheckInput();

	static bool temp[5];
	memset(temp, false, 5); // reset all elements to false
	if (m_app->inputManager.isKeyDown(SDLK_w)) temp[0] = true/*, m_app->mainSocket << UDPStream::packet << MOVE << 'w' << m_app->serverAddress*/;
	if (m_app->inputManager.isKeyDown(SDLK_a)) temp[2] = true/*, m_app->mainSocket << UDPStream::packet << MOVE << 'a' << m_app->serverAddress*/;
	if (m_app->inputManager.isKeyDown(SDLK_s)) temp[1] = true/*, m_app->mainSocket << UDPStream::packet << MOVE << 's' << m_app->serverAddress*/;
	if (m_app->inputManager.isKeyDown(SDLK_d)) temp[3] = true/*, m_app->mainSocket << UDPStream::packet << MOVE << 'd' << m_app->serverAddress*/;
	if (m_app->inputManager.isKeyDown(SDLK_SPACE)) temp[4] = true;
	m_carPhy.Update(temp, gameApp->deltaTime);
	glm::vec3 perFront = glm::vec3(-m_carPhy.front.z, 0.0f, m_carPhy.front.x);
	m_playerwheels[0].transform.position = m_player->transform.position + m_carPhy.front*2.0f + perFront*1.25f;
	m_playerwheels[1].transform.position = m_player->transform.position + m_carPhy.front*2.0f - perFront*1.25f;
	m_playerwheels[2].transform.position = m_player->transform.position - m_carPhy.front*2.0f + perFront*1.25f;
	m_playerwheels[3].transform.position = m_player->transform.position - m_carPhy.front*2.0f - perFront*1.25f;
	m_playerwheels[0].transform.rotation = m_player->transform.rotation - glm::vec3(0.0f, (m_carPhy.steerAngle*180.0f) / M_PI, 0.0f);
	m_playerwheels[1].transform.rotation = m_player->transform.rotation - glm::vec3(0.0f, (m_carPhy.steerAngle*180.0f) / M_PI, 0.0f);
	m_playerwheels[2].transform.rotation = m_player->transform.rotation;
	m_playerwheels[3].transform.rotation = m_player->transform.rotation;

	//std::cout << m_player->transform.position.x << "," << m_player->transform.position.y << std::endl;

	m_camera.Translate(m_player->transform.position - (m_carPhy.front*35.0f) + glm::vec3(0.0f,15.0f, 0.0f));
	m_camera.SetTarget(glm::vec3{ 0,2,0 } +m_player->transform.position);

	// Update text nick plane
	m_textNick.position = m_player->transform.position + glm::vec3{ 0,3,0 };
	m_textNick.rotation = m_player->transform.rotation;

	if (m_app->inputManager.isKeyPressed(SDLK_ESCAPE)) m_app->ChangeScreen(MENU_SCREEN);
}

void MultiplayerScreen::CheckInput(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt)) {
		m_app->OnSDLEvent(evnt);
		if (evnt.type == SDL_WINDOWEVENT) {
			switch (evnt.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
				SDL_GetWindowSize(m_app->window.SDLWindow, &m_app->screenWidth, &m_app->screenHeight);
				glViewport(0, 0, m_app->screenWidth, m_app->screenHeight); // Set the OpenGL viewport to window dimensions
				int nw = (m_app->screenHeight * FIXED_ASPECT_RATIO);
				m_camera.Resize(nw + (m_app->screenWidth - nw) / 2, m_app->screenHeight);
				break;
			}
		}
	}
	if (m_app->inputManager.isKeyPressed(SDLK_e)) RendererList::DEBUG_DRAW = !RendererList::DEBUG_DRAW;
	if (m_app->inputManager.isKeyPressed(SDLK_q)) RendererList::DEBUG_MODE = (RendererList::DEBUG_MODE == GL_TRIANGLES) ? GL_LINES : GL_TRIANGLES;
}

void MultiplayerScreen::Draw(void) {
	m_mainProgram.bind();
		m_renderer.DrawObjects(m_mainProgram, m_camera);
		m_textNick.Draw(m_mainProgram, m_app->font);
	m_mainProgram.unbind();

	if (RendererList::DEBUG_DRAW)
		m_lightProgram.bind(),
			m_renderer.DrawDebug(m_lightProgram, m_camera),
		m_lightProgram.unbind();

	m_app->window.swapBuffer(); // Swap OpenGL buffers if double-buffering is supported
}