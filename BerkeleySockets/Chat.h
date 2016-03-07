#pragma once
#include <SocketsLib/UDPSocket.h>
#include <memory>
#include "MsgManager.h"

class Chat
{
protected:
	static const int MAX_DATA = 1300;
	SocketAddress m_addr;
	std::shared_ptr<MsgManager> m_msgManager;
	std::shared_ptr<UDPSocket> m_socket;
public:
	
	Chat(const std::string &s, const std::shared_ptr<MsgManager> &m) :
		m_addr(s),
		m_msgManager(m),
		m_socket(std::make_shared<UDPSocket>()) 
	{}

	virtual ~Chat() = default;
	
	virtual void operator() () = 0;	
};

