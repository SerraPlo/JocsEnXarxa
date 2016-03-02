#pragma once
#include "Chat.h"

class Sender : public Chat
{
	std::mutex m_blocker;
public:
	Sender(const std::string &s) {
		m_addr.setAddress(s);
	}
	~Sender() = default;
	void operator()() {
		const int MAX_DATA = 1300;
		char data[MAX_DATA];
		while (std::cin.getline(data, MAX_DATA)) {
			m_socket->SendTo(data, MAX_DATA, m_addr);
			
			m_blocker.lock();
			std::string temp = data;
			m_msgPool->push_back("me: " + temp);
			//printf("%s\n", data);
			m_blocker.unlock();

			if (!strcmp(data, "exit")) break;
		}
	}
};

