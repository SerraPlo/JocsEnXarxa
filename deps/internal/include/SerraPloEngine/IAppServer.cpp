#include "IAppServer.h"
#include "Timing.h"

namespace SerraPlo {

	IAppServer::IAppServer(int port) : dispatcher(port) {};
	
	IAppServer::~IAppServer() {
		//for (auto client : clientList) if (client.second) delete client.second, client.second = nullptr;
	}

	void IAppServer::Init() {

	}

	void IAppServer::Update() {
		try {
			int header;
			sockaddr sender;
			dispatcher >> UDPStream::packet >> sender >> header;
			std::cout << "dispatched" << std::endl;
			switch (header) {
				case LOGIN: {
					std::string nick;
					dispatcher >> nick;
					clientList[sender.hash] = { sender, nick, ScreenState::NONE, SCREEN_INDEX_NO_SCREEN };
					std::cout << nick << " is logged in" << std::endl;
				} break; 
				case MOVE: {
					char pressed_key;
					dispatcher >> pressed_key;
					std::cout << clientList[sender.hash].nick << " is moving " << std::endl;
					switch (pressed_key) {
						case 'w':
						std::cout << "w pressed" << std::endl;
					}
				}
			}
			for (auto variable : clientList) {
				//dispatcher << UDPStream::packet << variable.second.nick << std::vector<int>({ 0,1,2,3 }) << variable.second.pos; 
				//for (auto eachPlayer : clientList) dispatcher << eachPlayer.second.player;
			}
		} catch (UDPStream::wrong) { //if the amount of packet data not corresponding to the amount of data that we are trying to read
			std::cout << "Client Received wrongly serialized data" << std::endl;
		} catch (UDPStream::empty) {} //if the package is empty or have not received anything
	}

	void IAppServer::Run() {
		Init(); // Call the init everything function
		FPSLimiter fpsLimiter; // Spawn the main instance of the timing limiter
		fpsLimiter.setTargetFPS(TARGET_FPS); // Set the frames per second we whish to have, ideally 60-120

		while (m_isRunning) { // While game is running
			fpsLimiter.begin();			// Init FPS counter
			Update();					// Main update function
			fps = fpsLimiter.fps;		// Get the current fps of the class instance
			deltaTime = fpsLimiter.deltaTime;
			fpsLimiter.end();			// Calculate and restore FPS
		}
	}

	void IAppServer::ExitGame() {
		m_isRunning = false; // Execution ends
	}

}
