#pragma once
#include <SerraPloEngine/IApp.h>
#include "ClientProxy.h"
#include <map>
#pragma comment(lib, "SerraPloEngine.lib")

#define PORT 5000
#define MAX_PARTY_PLAYERS 4
#define MAX_PARTIES 3

class AppServer : public IApp {
	UDPStream dispatcher;
	std::vector<std::map<uint64_t, ClientProxy*>> clientLists;
	int numberClientLists = 0;
	float m_counterUpdate[MAX_PARTIES];
	float m_aliveCounter[MAX_PARTIES];

	explicit AppServer() : dispatcher(PORT) {};
	AppServer(AppServer const&) = delete;
	void operator=(AppServer const&) = delete;
	~AppServer() { AppServer::Destroy(); }

	// Initialize everything related to game internals
	virtual void Init(void) override;
	void Send(void);
	void Receive(void);
	
public:
	static AppServer& Instance() {
		static AppServer instance;
		return instance;
	}
	// Where magic occurs, to be used to play the whole game
	virtual void Run(void) override;
	// Destroy screen list and set game running to false
	virtual void Destroy(void);
};