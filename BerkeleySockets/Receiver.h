#pragma once
#include "Chat.h"

class Receiver : public Chat
{
public:
	explicit Receiver(const std::string &s) : Chat(s) { m_socket->Bind(m_addr); }
	~Receiver() = default;

	void operator()() override {
		char data[MAX_DATA];
		SocketAddress from;
		std::mutex m_blocker;

		while (true) {
			m_socket->ReceiveFrom(data, MAX_DATA, from);
			
			m_blocker.lock();
			m_msgPool.push_back(!strcmp(data, "exit") ? "User disconnected" : data);
			std::cout << '>' << data << std::endl;
			m_blocker.unlock();
			
			if (!strcmp(data, "exit")) break;
		}
	}
};

