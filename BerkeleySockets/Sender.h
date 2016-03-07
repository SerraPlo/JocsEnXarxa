#pragma once
#include "Chat.h"

class Sender : public Chat
{
public:
	explicit Sender(const std::string &s, MsgManager* m) : Chat(s, m) {}
	~Sender() = default;

	void operator()() override {
		char data[MAX_DATA];
		static std::mutex m_blocker;
		while (true) {
			std::cin.getline(data, MAX_DATA);
			m_socket->SendTo(data, MAX_DATA, m_addr);
			std::string temp = data;
			m_msgManager->addMsg("Me: " + temp);
			m_msgManager->PrintMsg();
			if (!strcmp(data, "exit")) break;
		}
	}
};

