#pragma once
#include "ChatAgent.h"

class Receiver : public ChatAgent
{
public:
	explicit Receiver(const std::string &s, MsgManager &m) : ChatAgent(s, m) {
		m_socket->Bind(m_addr); 
	}
	~Receiver() = default;

	void operator()() override {
		char data[MAX_DATA];
		SocketAddress from;
		while (true) {
			m_socket->ReceiveFrom(data, MAX_DATA, from);
			m_msgManager.addMsg(!strcmp(data, "exit") ? "User disconnected" : data);
			m_msgManager.PrintMsg(false);
			if (!strcmp(data, "exit")) break;
		}
	}
};

