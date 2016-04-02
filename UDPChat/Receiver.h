#pragma once
#include "ChatAgent.h"

class Receiver : public ChatAgent
{
public:
	explicit Receiver(const std::string &s, MsgManager &m) : ChatAgent(s, m) {
		m_socket->Bind(m_addr); 
	}
	Receiver(const Receiver& other) = default;
	~Receiver() = default;

	Receiver& operator=(const Receiver& other) = default;
	void operator()() override {
		char data[MAX_BYTES];
		SocketAddress from;
		while (true) {
			m_socket->ReceiveFrom(data, MAX_BYTES, from);
			m_msgManager.addMsg(!strcmp(data, "exit") ? "User disconnected" : data);
			m_msgManager.PrintMsg(false);
			if (!strcmp(data, "exit")) break;
		}
	}
};

