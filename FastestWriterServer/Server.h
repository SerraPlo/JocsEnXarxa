#pragma once
#include <TCPSocket.h>
#include "ClientProxy.h"
#include "ListWords.h"

class Server
{
	const int m_numPlayers;
	SocketAddress m_addr;
	TCPSocket m_dispatcher;
	ListWords m_wordsList;
	std::vector<ClientProxy> m_clientList;

	void SendToAll(const char* data);
	bool ProcessMsg(int id, const std::string &data);
	void InitConnection(void);
	void SetNicks(void);
	void GameLoop(void);
public:
	explicit Server(const char* bindAddress, int numPlayers);
	~Server() = default;

	void Run(void);
};

