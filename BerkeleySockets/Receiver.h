#pragma once
#include "Chat.h"

class Receiver : public Chat
{
	std::mutex m_blocker;
public:
	Receiver(const std::string &s) {
		m_addr.setAddress(s);
	}
	~Receiver() = default;
	void operator()() {
		const int MAX_DATA = 1300;
		m_socket->Bind(m_addr);
		char data[MAX_DATA];
		while (1) {
			SocketAddress from;
			m_socket->ReceiveFrom(data, MAX_DATA, from);
			
			m_blocker.lock();
			if (!strcmp(data, "exit")) m_msgPool->push_back("user disconnected");
			else m_msgPool->push_back(data);
			//printf("%s\n", data);
			m_blocker.unlock();
			
			if (!strcmp(data, "exit")) break;
		}
	}
};

