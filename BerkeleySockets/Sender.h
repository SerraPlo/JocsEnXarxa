#pragma once
#include "ChatAgent.h"


class Sender : public ChatAgent
{
public:
	explicit Sender(const std::string &s, MsgManager &m) : ChatAgent(s, m) {}
	~Sender() = default;

	void operator()() override {
		char data[MAX_DATA];
		while (true) {
			std::cin.getline(data, MAX_DATA);
			//Utils::ReadChar(data);
			m_socket->SendTo(data, MAX_DATA, m_addr);
			m_msgManager.addMsg("Me: " + std::string(data));
			m_msgManager.PrintMsg(true);
			if (!strcmp(data, "exit")) exit(EXIT_SUCCESS); //trust on guillermo
		}
	}
};

