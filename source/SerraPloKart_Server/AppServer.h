#pragma once
#include <SerraPloEngine/IApp.h>
#include "ClientProxy.h"
#include <map>
#pragma comment(lib, "SerraPloEngine.lib")

#define PORT 5000

class AppServer : public IApp {
	UDPStream dispatcher;
	std::map<uint64_t, ClientProxy> clientList;

	explicit AppServer() : dispatcher(PORT) {};
	AppServer(AppServer const&) = delete;
	void operator=(AppServer const&) = delete;
	~AppServer() = default;

	// Initialize everything related to game internals
	virtual void Init() override;
	// Main update function of the game
	void Update();
public:
	static AppServer& Instance() {
		static AppServer instance;
		return instance;
	}

	// Where magic occurs, to be used to play the whole game
	virtual void Run() override;
	// Destroy screen list and set game running to false
	virtual void Exit() override;
};