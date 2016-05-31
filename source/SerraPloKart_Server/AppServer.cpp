#include <SerraPloEngine/Timing.h>
#include "AppServer.h"
#include <ctime>

void AppServer::Init(void) {
	m_aliveCounter = float(clock());
	m_counterUpdate = float(clock());
}

void AppServer::Update(void) {
	try {
		if (clock() > m_aliveCounter + MS_ALIVE_DELAY) {
			std::cout << "Sending alive..." << std::endl;
			dispatcher << UDPStream::packet << MSG_ALIVE;
			for (auto &client : clientList) dispatcher << client.second->address;
			m_aliveCounter = float(clock());
		}
		if (clock() >= m_counterUpdate + 100.0f && !clientList.empty()) {
			for (auto &client : clientList) {
				dispatcher << UDPStream::packet << MSG_UPDATE << client.second->nick << client.second->transform.position.x << client.second->transform.position.z << client.second->transform.rotation.y;
				for (auto &client2 : clientList) dispatcher << client2.second->address;
			}
			m_counterUpdate = float(clock());
		}
		int header;
		sockaddr sender;
		dispatcher >> UDPStream::packet >> sender >> header;
		switch (header) {
			case MSG_LOGIN: {
				std::string nick;
				dispatcher >> nick;
				if (clientList.find(sender.hash) == clientList.end()) {
					clientList[sender.hash] = new ClientProxy(sender,nick);
					clientList[sender.hash]->carPhy.AddTransform(&clientList[sender.hash]->transform);
					std::cout << nick << " has logged in. Added to client database." << std::endl;
					dispatcher << UDPStream::packet << MSG_BEGIN << sender;
				}
			} break;
			case MSG_EXIT: {
				auto &it = clientList.find(sender.hash);
				std::cout << it->second->nick << " has been disconnected." << std::endl;
				clientList.erase(it);   //<----------------------------------------------------------------------------TODO : maybe
			} break;
			case MSG_UPDATE: {
				input10 input;
				float ccX[10]; float ccY[10];
				dispatcher >> input.w >> input.a >> input.s >> input.d >>ccX>>ccY>> input.dt ;
				bool temp[5];
				for (int i = 0; i < 10; i++) {
					//memset(temp, false, 5);
					temp[0] = input.w[i]; temp[1] = input.a[i];
					temp[2] = input.s[i]; temp[3] = input.d[i];
					temp[4] = false;
					clientList[sender.hash]->carPhy.Update(temp, input.dt[i], glm::vec2(ccX[i],ccY[i]));
				}
				//std::cout << clientList[sender.hash].transform.position.x << "," << clientList[sender.hash].transform.position.z << std::endl;
				//std::cout << clientList[sender.hash].transform.rotation.y << std::endl;
			}
		}
		if (clientList.empty()) std::cout << "All players disconnected. Shutting down..." << std::endl, system("pause"); ///TODO
	} catch (UDPStream::wrong) { //if the amount of packet data not corresponding to the amount of data that we are trying to read
		std::cout << "--> ALERT: Wrongly serialized data received!" << std::endl;
	} catch (UDPStream::empty) {} //if the package is empty or have not received anything
}

void AppServer::Run(void) {
	Init(); // Call the init everything function
	FPSLimiter fpsLimiter; // Spawn the main instance of the timing limiter
	fpsLimiter.setTargetFPS(TARGET_FPS); // Set the frames per second we whish to have, ideally 60-120
	
	std::cout << "//////////////////////////////" << std::endl;
	std::cout << "//  SerraPlo's Kart Server  //" << std::endl;
	std::cout << "//////////////////////////////" << std::endl;
	std::cout << std::endl << "Listening to new player..." << std::endl;
	while (m_isRunning) {
		fpsLimiter.begin();			// Init FPS counter
		Update();
		fps = fpsLimiter.fps;		// Get the current fps of the class instance
		deltaTime = fpsLimiter.deltaTime;
		fpsLimiter.end();			// Calculate and restore FPS
	}
}

void AppServer::Destroy(void) {
	dispatcher << UDPStream::packet << MSG_EXIT;
	for (auto &client : clientList) dispatcher << client.second->address, delete client.second;
	m_isRunning = false; // Execution ends
}