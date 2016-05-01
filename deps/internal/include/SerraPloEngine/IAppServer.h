#pragma once
#include "IApp.h"
#include "IClientProxy.h"
#include <map>

namespace SerraPlo {

	class IAppServer : public IApp {
	protected:
		// Initialize everything related to game internals
		//virtual void Init() override;
		// Main update function of the game
		//virtual void Update() override;
	public:
		std::map<uint64_t, IClientProxy*> clientList;
		UDPStream server;

		explicit IAppServer(int port) : server(port) {};
		~IAppServer();

		// Where magic occurs, to be used to play the whole game
		virtual void Run() override;
		// Destroy screen list and set game running to false
		virtual void ExitGame() override;
	};

}