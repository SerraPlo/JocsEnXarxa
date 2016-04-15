#pragma once
#include <SocketLib/UDPSocket.h>
#include <memory>
#include "MsgManager.h"

#pragma comment(lib, "SocketLib")

class ChatAgent
{
protected:
	SocketAddress m_addr;
	MsgManager &m_msgManager;
	std::shared_ptr<UDPSocket> m_socket;
public:
	
	ChatAgent(const std::string &s, MsgManager &m) :
		m_addr(s),
		m_msgManager(m),
		m_socket(std::make_shared<UDPSocket>())
	{}
	ChatAgent(const ChatAgent& other) = default;
	virtual ~ChatAgent() = default;
	
	ChatAgent& operator=(const ChatAgent& other) = default;
	virtual void operator() () = 0;	
};

