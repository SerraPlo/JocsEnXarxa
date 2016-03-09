#pragma once
#include <string>
#include <memory>
#include <TCPSocket.h>

class ClientProxy
{
	std::string m_nick = "Dummy";
	int m_score = 0;
	std::shared_ptr<TCPSocket> m_tcpSocket = std::make_shared<TCPSocket>();
public:
	ClientProxy() = default;
	~ClientProxy() = default;
};

