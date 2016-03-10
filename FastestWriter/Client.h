#pragma once
#include <TCPSocket.h>

class Client
{
	SocketAddress m_addr; //store server address to connect
	TCPSocket m_tcpSocket; //main client socket
	const std::string m_nick; //client's nickname

	bool ProcessMsg(const std::string &data); //operate according to message key
	void CheckBegin(void) const; //check if game begins, then send the nickname
	void GameLoop(void); //main player's game loop
public:
	explicit Client(const char* serverAddress, const std::string &nick);
	~Client() = default;

	void Run(void);
};

