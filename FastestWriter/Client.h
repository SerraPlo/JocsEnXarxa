#pragma once
#include <TCPSocket.h>
#include "UserData.h"

class Client
{
	SocketAddress m_addr; //store server address to connect
	TCPSocket m_tcpSocket; //main client socket
	const std::string m_nick; //client's nickname
	UserData &m_data;

	bool ProcessMsg(const std::string &data); //operate according to message key
	void CheckBegin(void); //check if game begins, then send the nickname
	void GameLoop(void); //main player's game loop
public:
	explicit Client(const char* serverAddress, const std::string &nick, UserData &uD);
	~Client() = default;

	void Run(void);
};

