#pragma once
#include <SocketsLib/UDPSocket.h>
#include <memory>
#include <vector>
#include <mutex>
#include <iostream>

class Chat
{
protected:
	static const int MAX_DATA = 1300;

	SocketAddress m_addr;
	std::shared_ptr<UDPSocket> m_socket;
	std::vector<std::string> m_msgPool;
public:
	Chat(const std::string &s) :
		m_addr(s),
		m_socket(std::make_shared<UDPSocket>()) {}
	virtual ~Chat() = default;

	virtual void operator() () = 0;
};

