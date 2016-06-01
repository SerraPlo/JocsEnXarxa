#include <SerraPloEngine/PathLoader.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "MultiplayerScreen.h"
#include "AppClient.h"
#include <ctime>

#define FIXED_ASPECT_RATIO 16 / 9

void MultiplayerScreen::Build(void) {
	m_app = dynamic_cast<AppClient*>(gameApp);

	// Initialize camera with viewport dimensions
	int nw = (m_app->screenHeight * FIXED_ASPECT_RATIO);
	m_camera.Resize(nw + (m_app->screenWidth - nw) / 2, m_app->screenHeight);

	//Initialize main shaders
	m_mainProgram.LoadShaders("shaders/main.vert", "shaders/main.frag");
	m_textProgram.LoadShaders("shaders/text.vert", "shaders/text.frag");
	//Initialize debug shaders
	m_debugProgram.LoadShaders("shaders/debug.vert", "shaders/debug.frag");

	// Init IA Path with 12 points for steering path following
	m_aiPath = {
		{ 40, 108 },
		{ 10, 80 },
		{ -3, 7 },
		{ -40, -43 },
		{ -35, -70 },
		{ 10, -108 },
		{ 40, -100 },
		{ 165, 10 },
		{ 290, 20 },
		{ 315, 50 },
		{ 308, 90 },
		{ 280, 108 }
	};

	// Init game physics
	m_carPhysics.AddTransform(&m_player.body.transform);
	m_aiPhysics.AddAICar(&debugIA1.transform, 0.8f, 200.0f * 60.0f);
	m_aiPhysics.AddAICar(&debugIA2.transform, 0.9f, 200.0f * 60.0f);
	m_aiPhysics.AddPath(&m_aiPath);
}

void MultiplayerScreen::Destroy(void) {

}

void MultiplayerScreen::OnEntry(void) {
	//SDL_ShowCursor(0);
	m_app->aliveCounter = float(clock());

	// Set player nick to text plane
	if (!m_player.nickIdentifier.textureid) {
		m_player.nickIdentifier.SetText(m_app->nick, { 255, 100, 255 }, m_app->font);
		m_player.nickIdentifier.scale = { 2,1,2 };
	}

	// Load player base kart model
	m_player.body.transform.position = { 180, 0, 115 };
	m_player.body.transform.rotation = { 0, -90, 0 };
	m_player.body.meshRef = &m_app->assetManager.FindMesh("mesh_kart_default");
	m_player.body.materialRef = &m_app->assetManager.FindMaterial("material_kart_default");
	m_player.body.materialRef->materialData[0].shininess = 50;
	m_player.body.materialRef->materialData[0].specular = {1,1,1};
	m_renderer.AddObject(&m_player.body);

	// Load player kart wheels
	for (int i = 0; i < 4; ++i)
		m_player.wheels[i].meshRef = &m_app->assetManager.FindMesh("mesh_wheel"),
		m_player.wheels[i].materialRef = &m_app->assetManager.FindMaterial("material_blue"),
		m_renderer.AddObject(&m_player.wheels[i]);

	/// TODO: set emissive color !! material parameters!
	skybox.transform.position = { 0, -100, 0 };
	skybox.transform.scale = { 3, 3, 3 };
	skybox.meshRef = &m_app->assetManager.FindMesh("mesh_skybox");
	skybox.materialRef = &m_app->assetManager.FindMaterial("material_skybox");
	m_renderer.AddObject(&skybox);

	circuit.transform.scale = { 1, 1, 0.2 };
	circuit.meshRef = &m_app->assetManager.FindMesh("mesh_circuit");
	circuit.materialRef = &m_app->assetManager.FindMaterial("material_circuit");
	m_renderer.AddObject(&circuit);
	
	debugCollisions.color = { 0,0.5, 0 };
	debugCollisions.meshRef = &m_app->assetManager.FindMesh("mesh_debug_collisions");
	m_renderer.AddObject(&debugCollisions);

	// Init IA debug
	debugIA1.transform.position = { 200, 0, 115 };
	debugIA1.transform.rotation = { 0, -90, 0 };
	debugIA1.meshRef = &m_app->assetManager.FindMesh("mesh_kart_default");
	debugIA1.materialRef = &m_app->assetManager.FindMaterial("material_red");
	debugIA1.materialRef->materialData[0].shininess = 50;
	debugIA1.materialRef->materialData[0].specular = { 1,1,1 };
	m_renderer.AddObject(&debugIA1);
	debugIA2.transform.position = { 200, 0, 105 };
	debugIA2.transform.rotation = { 0, -90, 0 };
	debugIA2.meshRef = &m_app->assetManager.FindMesh("mesh_kart_default");
	debugIA2.materialRef = &m_app->assetManager.FindMaterial("material_green");
	debugIA2.materialRef->materialData[0].shininess = 50;
	debugIA2.materialRef->materialData[0].specular = { 1,1,1 };
	m_renderer.AddObject(&debugIA2);

	// Init camera
	m_camera.Translate(m_player.body.transform.position + glm::vec3{35, 0, 0} + glm::vec3(0.0f, 15.0f, 0.0f));
	m_camera.SetTarget(glm::vec3{ 0,2,0 } + m_player.body.transform.position);

	// LIGHTNING
	// Init directional light
	m_dirLight.direction = { -0.2f, -1.0f, -0.3f };
	m_dirLight.ambient = { 0.05f, 0.05f, 0.15f };
	m_dirLight.diffuse = { 0.1f, 0.1f, 0.2f };
	m_dirLight.specular = { 0.1f, 0.1f, 0.1f };
	m_renderer.AddLight(&m_dirLight);

	// Init point lights
	m_pointLights[0].position = { 150, 0, 100 };
	m_pointLights[1].position = { 150, 0, 120 };
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i) {
		m_pointLights[i].ambient = { 1.0f, 0.3f, 0.3f };
		m_pointLights[i].diffuse = { 1.0f, 0.3f, 0.3f };
		m_pointLights[i].specular = { 1.0f, 1.0f, 1.0f };
		m_pointLights[i].constant = 1.0f;
		m_pointLights[i].linear = 0.09f;
		m_pointLights[i].quadratic = 0.032f;
		m_renderer.AddLight(&m_pointLights[i]);
	}

	// Init spot lights
	m_spotLights[0].position = { 40, 8, 108 };
	m_spotLights[1].position = { 10, 8, 80 };
	m_spotLights[2].position = { -3, 8, 7 };
	m_spotLights[3].position = { -40, 8, -43 };
	m_spotLights[4].position = { -35, 8, -70 };
	m_spotLights[5].position = { 10, 8, -108 };
	m_spotLights[6].position = { 40, 8, -100 };
	m_spotLights[7].position = { 165, 8, 10 };
	m_spotLights[8].position = { 290, 8, 20 };
	m_spotLights[9].position = { 315, 8, 50 };
	m_spotLights[10].position = { 308, 8, 90 };
	m_spotLights[11].position = { 280, 8, 108 };
	for (int i = 0; i < MAX_SPOT_LIGHTS; ++i) {
		m_spotLights[i].direction = { 0, -1, 0 };
		m_spotLights[i].ambient = { 1.0f, 1.0f, 1.0f };
		m_spotLights[i].diffuse = { 1.0f, 1.0f, 0.7f };
		m_spotLights[i].specular = { 1.0f, 1.0f, 1.0f };
		m_spotLights[i].constant = 1.0f;
		m_spotLights[i].linear = 0.09f;
		m_spotLights[i].quadratic = 0.032f;
		m_spotLights[i].cutOff = glm::cos(glm::radians(50.0f));
		m_spotLights[i].outerCutOff = glm::cos(glm::radians(60.0f));
		m_renderer.AddLight(&m_spotLights[i]);
	}

	m_carLights.position = m_player.body.transform.position;
	m_carLights.direction = { -1, 0, 0 };
	m_carLights.ambient = { 1.0f, 1.0f, 1.0f };
	m_carLights.diffuse = { 1.0f, 1.0f, 0.5f };
	m_carLights.specular = { 1.0f, 1.0f, 1.0f };
	m_carLights.constant = 1.0f;
	m_carLights.linear = 0.027f;
	m_carLights.quadratic = 0.0028f;
	m_carLights.cutOff = glm::cos(glm::radians(30.0f));
	m_carLights.outerCutOff = glm::cos(glm::radians(40.0f));
	m_renderer.AddLight(&m_carLights, false);

	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0

	// Send light and material attributes to fragment shader
	m_renderer.SendStaticLightAttributes(m_mainProgram, m_camera);
	m_renderer.SendMaterialAttributes(m_mainProgram, m_camera);
}

void MultiplayerScreen::OnExit(void) {
	m_renderer.Clear();
	m_enemies.clear();
}

void MultiplayerScreen::CheckInput(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt)) m_app->OnSDLEvent(evnt);
	if (m_app->inputManager.isKeyPressed(SDLK_e)) RendererList::DEBUG_DRAW = !RendererList::DEBUG_DRAW;
	if (m_app->inputManager.isKeyPressed(SDLK_q)) RendererList::WIREFRAME_MODE = !RendererList::WIREFRAME_MODE;
}

void MultiplayerScreen::ProcessMsgs(void) {
	try {
		int header;
		m_app->mainSocket >> UDPStream::packet >> header;
		switch (header) {
			case MSG_BEGIN: { // Receive all enemies info and begin the game
				int maxPlayers;
				m_app->mainSocket >> maxPlayers;
				std::cout << "Max players:\t" << maxPlayers << std::endl;
				for (int i = 0; i < maxPlayers; ++i) {
					std::string nick;
					m_app->mainSocket >> nick;
					if (m_app->nick != nick) {
						auto &enemy = m_enemies[nick]; // Create enemy dynamically
						m_app->mainSocket >> enemy.body.transform.position >> enemy.body.transform.rotation; // Init its position and rotation
						std::cout << "New enemy:\t" << nick << std::endl;
						enemy.body.meshRef = &m_app->assetManager.FindMesh("mesh_kart_default");
						enemy.body.materialRef = &m_app->assetManager.FindMaterial("material_kart_0" + std::to_string(i % 9));
						enemy.body.materialRef->materialData[0].shininess = 50;
						enemy.body.materialRef->materialData[0].specular = { 1,1,1 };
						enemy.nickIdentifier.SetText(nick, { 100, 0, 100 }, m_app->font);
						m_renderer.AddObject(&enemy.body); /// Add body to renderer list ///TODO: add wheels
					} else m_app->mainSocket >> m_player.body.transform.position >> m_player.body.transform.rotation; // Set player position
				}
				for (int i = 0; i < MAX_POINT_LIGHTS; ++i) // Change red lights to green lights
					m_pointLights[i].ambient = { 0.3f, 1.0f, 0.3f },
					m_pointLights[i].diffuse = { 0.5f, 1.0f, 0.5f };
				m_renderer.SendStaticLightAttributes(m_mainProgram, m_camera);
				std::cout << "Game begins!" << std::endl;
			} break;
			case MSG_ALIVE: { // Check if server stills active
				//std::cout << "Server is alive" << std::endl;
				m_app->aliveCounter = float(clock());
			} break;
			case MSG_UPDATE: {
				for (int i = 0; i < m_enemies.size() + 1; ++i) {
					std::string nick;
					m_app->mainSocket >> nick;
					if (m_app->nick == nick) m_app->mainSocket >> myServTrans.position.x >> myServTrans.position.z >> myServTrans.rotation.y;
					else m_app->mainSocket >> m_enemies[nick].targetTransform.position.x >> m_enemies[nick].targetTransform.position.z >> m_enemies[nick].targetTransform.rotation.y;
				}
			} break;
			default: break;
		}
	} catch (UDPStream::wrong) { //if the amount of packet data not corresponding to the amount of data that we are trying to read
		std::cout << "--> ALERT: Wrongly serialized data received!" << std::endl;
	} catch (UDPStream::empty) {} //if the package is empty or have not received anything

	/*if (clock() > m_aliveCounter + MS_ALIVE_DELAY+1000 && currentScreen->screenIndex == SCREEN_MULTIPLAYER)
	std::cout << "Server closed. Disconecting..." << std::endl, nick.clear(), ChangeScreen(SCREEN_MENU);*/
}


void MultiplayerScreen::UpdateEnemies(void) {
	for (auto &enemy : m_enemies) {
		//bad interpolation
		enemy.second.body.transform.position += (enemy.second.targetTransform.position - enemy.second.body.transform.position)/4.0f;
		enemy.second.body.transform.rotation += (enemy.second.targetTransform.rotation - enemy.second.body.transform.rotation)/4.0f;
		
		glm::vec3 f = glm::vec3(sin((enemy.second.body.transform.rotation.y*M_PI) / 180), 0.0f, cos((enemy.second.body.transform.rotation.y*M_PI) / 180));
		glm::vec3 pF = glm::vec3(-f.z, 0.0f, f.x);
		enemy.second.wheels[0].transform.position = enemy.second.body.transform.position + f*2.0f + pF*1.25f;
		enemy.second.wheels[1].transform.position = enemy.second.body.transform.position + f*2.0f - pF*1.25f;
		enemy.second.wheels[2].transform.position = enemy.second.body.transform.position - f*2.0f + pF*1.25f;
		enemy.second.wheels[3].transform.position = enemy.second.body.transform.position - f*2.0f - pF*1.25f;

		enemy.second.wheels[0].transform.rotation = enemy.second.body.transform.rotation;
		enemy.second.wheels[1].transform.rotation = enemy.second.body.transform.rotation;
		enemy.second.wheels[2].transform.rotation = enemy.second.body.transform.rotation;
		enemy.second.wheels[3].transform.rotation = enemy.second.body.transform.rotation;

		enemy.second.nickIdentifier.position = enemy.second.body.transform.position + glm::vec3{ 0,4,0 };;
		enemy.second.nickIdentifier.rotation = enemy.second.body.transform.rotation;
	}
}

void MultiplayerScreen::DoPhysics(void) {
	//input
	static int m_inputCounter = 0;
	static float ccX[10];
	static float ccY[10];
	static input10 m_in2send;
	static bool temp[5];
	memset(temp, false, 5); // reset all elements to false
	if (m_app->inputManager.isKeyDown(SDLK_w)) temp[0] = true;
	if (m_app->inputManager.isKeyDown(SDLK_a)) temp[2] = true;
	if (m_app->inputManager.isKeyDown(SDLK_s)) temp[1] = true;
	if (m_app->inputManager.isKeyDown(SDLK_d)) temp[3] = true;
	if (m_app->inputManager.isKeyDown(SDLK_SPACE)) temp[4] = true;

	//Update
	std::vector<glm::vec3> enemiesPos;
	for (auto &enemy : m_enemies) {
		enemiesPos.push_back(enemy.second.body.transform.position);
	}
	//std::cout << m_app->myServTrans.position.x << std::endl;
	glm::vec2 vecColCar = m_carPhysics.ColideCars(enemiesPos);
	if (glm::length(m_player.body.transform.position - myServTrans.position) >= 15.0f) {
		m_player.body.transform.position = myServTrans.position;
		std::cout << "///////////////////////////PositionCorrected" << std::endl;
	}
	m_carPhysics.Update(temp, gameApp->deltaTime, vecColCar);
	
	//Send to server
	m_in2send.w[m_inputCounter] = temp[0]; m_in2send.a[m_inputCounter] = temp[1];
	m_in2send.s[m_inputCounter] = temp[2]; m_in2send.d[m_inputCounter] = temp[3];
	m_in2send.dt[m_inputCounter] = m_app->deltaTime;
	ccX[m_inputCounter] = vecColCar.x; ccY[m_inputCounter] = vecColCar.y;
	m_inputCounter++;
	if (m_inputCounter >= 10) {//send cada 10 updates
		m_inputCounter = 0;
		m_app->mainSocket << UDPStream::packet << MSG_UPDATE << m_in2send.w << m_in2send.a << m_in2send.s << m_in2send.d << m_in2send.dt << ccX << ccY << m_app->serverAddress;
		//std::cout << m_player->transform.position.x <<","<< m_player->transform.position.z<< std::endl;
	}
	//Extras position
		//Wheels
	glm::vec3 perFront = glm::vec3(-m_carPhysics.front.z, 0.0f, m_carPhysics.front.x);
	m_player.wheels[0].transform.position = m_player.body.transform.position + m_carPhysics.front*.5f + perFront*1.5f;
	m_player.wheels[1].transform.position = m_player.body.transform.position + m_carPhysics.front*.5f - perFront*1.5f;
	m_player.wheels[2].transform.position = m_player.body.transform.position - m_carPhysics.front*2.0f + perFront*1.5f;
	m_player.wheels[3].transform.position = m_player.body.transform.position - m_carPhysics.front*2.0f - perFront*1.5f;
	m_player.wheels[0].transform.rotation = m_player.body.transform.rotation - glm::vec3(0.0f, (m_carPhysics.steerAngle*180.0f) / M_PI, 0.0f);
	m_player.wheels[1].transform.rotation = m_player.body.transform.rotation - glm::vec3(0.0f, (m_carPhysics.steerAngle*180.0f) / M_PI, 0.0f);
	m_player.wheels[2].transform.rotation = m_player.body.transform.rotation;
	m_player.wheels[3].transform.rotation = m_player.body.transform.rotation;
	//std::cout << m_player->transform.position.x << "," << m_player->transform.position.z << std::endl;
		//camera
	m_camera.Translate(m_player.body.transform.position - (m_carPhysics.front*35.0f) + glm::vec3(0.0f,15.0f, 0.0f));
	m_camera.SetTarget(glm::vec3{ 0,2,0 } + m_player.body.transform.position);
		//text
	m_player.nickIdentifier.position = m_player.body.transform.position + glm::vec3{ 0,4,0 };
	m_player.nickIdentifier.rotation = m_player.body.transform.rotation;
		//Enemies
	UpdateEnemies();
	
	//ESC
	if (m_app->inputManager.isKeyPressed(SDLK_ESCAPE)) m_app->ChangeScreen(SCREEN_MENU);

	//Update car light position & direction
	m_carLights.position = m_player.body.transform.position + m_carPhysics.front*2.0f + glm::vec3{ 0,1,0 };
	m_carLights.direction = m_carPhysics.front - glm::vec3{ 0,0.3f,0 };

	//IA update
	m_aiPhysics.Update(gameApp->deltaTime);
}

void MultiplayerScreen::Update() {
	ProcessMsgs();
	CheckInput();
	if (!m_enemies.empty()) DoPhysics();
}

void MultiplayerScreen::Draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	m_renderer.DrawObjects(m_mainProgram, m_camera);

	m_textProgram.Bind();
	m_player.nickIdentifier.Draw(m_textProgram, m_camera);
	for (auto &enemy : m_enemies) enemy.second.nickIdentifier.Draw(m_textProgram, m_camera);

	if (RendererList::DEBUG_DRAW) m_renderer.DrawDebug(m_debugProgram, m_camera);

	m_app->window.swapBuffer(); // Swap OpenGL buffers if double-buffering is supported
}