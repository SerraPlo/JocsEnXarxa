#pragma once
#include <TCPSocket.h>

class Client
{
	SocketAddress m_addr;
	TCPSocket m_tcpSocket;
	const std::string m_nick;

	bool ProcessMsg(const std::string &data);
	void CheckBegin(void) const;
	void GameLoop(void);
public:
	explicit Client(const char* serverAddress, const std::string &nick);
	~Client() = default;

	void Run(void);
};

