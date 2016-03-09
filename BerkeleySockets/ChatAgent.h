#pragma once
#include <SocketsLib/UDPSocket.h>
#include <memory>
#include "MsgManager.h"

class ChatAgent
{
protected:
	static const int MAX_DATA = 1300;
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

