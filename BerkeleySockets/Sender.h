#pragma once
#include "Chat.h"

class Sender : public Chat
{
public:
	explicit Sender(const std::string &s) : Chat(s) {}
	~Sender() = default;

	void operator()() override {
		char data[MAX_DATA];
		std::mutex m_blocker;

		while (true) {
			std::cin.getline(data, MAX_DATA);
			m_socket->SendTo(data, MAX_DATA, m_addr);
			
			m_blocker.lock();
			std::string temp = data;
			m_msgPool.push_back("Me: " + temp);
			std::cout << '>' << temp << std::endl;
			m_blocker.unlock();

			if (!strcmp(data, "exit")) break;
		}
	}
};

