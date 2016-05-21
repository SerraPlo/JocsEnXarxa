#include <SerraPloEngine/Timing.h>
#include "AppServer.h"

void AppServer::Init() {

}

void AppServer::Update() {
	try {
		int header;
		sockaddr sender;
		dispatcher >> UDPStream::packet >> sender >> header;
		switch (header) {
			case LOGIN: {
				std::string nick;
				dispatcher >> nick;
				clientList[sender.hash] = { sender, nick, ScreenState::NONE, SCREEN_INDEX_NO_SCREEN };
				std::cout << nick << " has logged in. Added to client database." << std::endl;
				dispatcher << UDPStream::packet << BEGIN << sender;
			} break;
			case EXIT: {
				auto it = clientList.find(sender.hash);
				std::cout << it->second.nick << " has been disconnected." << std::endl;
				clientList.erase(it);
			} break;
			case MOVE: {
				char pressed_key;
				dispatcher >> pressed_key;
				switch (pressed_key) {
					case 'w': std::cout << clientList[sender.hash].nick << " is moving forward." << std::endl; break;
					case 'a': std::cout << clientList[sender.hash].nick << " is turning left." << std::endl; break;
					case 's': std::cout << clientList[sender.hash].nick << " is braking." << std::endl; break;
					case 'd': std::cout << clientList[sender.hash].nick << " is turning right." << std::endl;
				}
			}
		}
		if (clientList.empty()) std::cout << "All players disconnected. Shutting down..." << std::endl, system("pause"); ///TODO
		//for (auto client : clientList) {
			//SEND TO ALL
		//}
	} catch (UDPStream::wrong) { //if the amount of packet data not corresponding to the amount of data that we are trying to read
		std::cout << "--> ALERT: Wrongly serialized data received!" << std::endl;
	} catch (UDPStream::empty) {} //if the package is empty or have not received anything
}

void AppServer::Run() {
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

void AppServer::Exit() {
	m_isRunning = false; // Execution ends
}