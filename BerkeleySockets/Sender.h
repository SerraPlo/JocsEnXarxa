#pragma once
#include "Chat.h"

class Sender : public Chat
{
public:
	explicit Sender(const std::string &s, const std::shared_ptr<MsgManager> &m) : Chat(s, m) {}
	~Sender() = default;

	void operator()() override {
		char data[MAX_DATA];
		while (true) {
			std::cin.getline(data, MAX_DATA);
			m_socket->SendTo(data, MAX_DATA, m_addr);
			std::string temp = data;
			m_msgManager->addMsg("Me: " + temp);
			m_msgManager->PrintMsg(true);
			if (!strcmp(data, "exit")) exit(0); //trust guillermo
		}
	}
};

