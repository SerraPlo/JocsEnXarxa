#pragma once
#include "Chat.h"

class Receiver : public Chat
{
public:
	explicit Receiver(const std::string &s, MsgManager* m) : Chat(s, m) {
		m_socket->Bind(m_addr); 
	}
	~Receiver() = default;

	void operator()() override {
		char data[MAX_DATA];
		SocketAddress from;
		while (true) {
			m_socket->ReceiveFrom(data, MAX_DATA, from);
			m_msgManager->addMsg(!strcmp(data, "exit") ? "User disconnected" : data);
			m_msgManager->PrintMsg();
			if (!strcmp(data, "exit")) break;
		}
	}
};

