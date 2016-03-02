#pragma once
#include <SocketsLib/UDPSocket.h>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <mutex>

class Chat
{
protected:
	SocketAddress m_addr;
	std::shared_ptr<UDPSocket> m_socket;
	std::shared_ptr<std::vector<std::string>> m_msgPool;
public:
	Chat() = default;
	virtual ~Chat() = default;
};

