#pragma once
#include <TCPSocket.h>
#include "ClientProxy.h"
#include "ListWords.h"

class Server
{
	const int m_numPlayers; //max players to be connected
	SocketAddress m_addr; //main address to be binded to socket
	TCPSocket m_dispatcher; //main server's socket
	ListWords m_wordsList; //list of words to be displayed for the game
	std::vector<ClientProxy> m_clientList; //list of clients with its nickname, score and socket reference

	void SendToAll(const char* data); //send message to all clients
	bool ProcessMsg(int id, const std::string &data); //operate according to message key
	void InitConnection(void); //initialize connection for all the players
	void SetNicks(void); //store nicknames in list of clients
	void GameLoop(void); //main server's game loop
public:
	explicit Server(const char* bindAddress, int numPlayers);
	~Server() = default;

	void Run(void);
};

