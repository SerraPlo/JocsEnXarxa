#pragma once
#include "ChatAgent.h"


class Sender : public ChatAgent
{
public:
	explicit Sender(const std::string &s, MsgManager &m) : ChatAgent(s, m) {}
	Sender(const Sender& other) = default;
	~Sender() = default;

	Sender& operator=(const Sender& other) = default;
	void operator()() override {
		char data[MAX_BYTES];
		while (true) {
			std::cin.getline(data, MAX_BYTES);
			//Utils::ReadChar(data);
			m_socket->SendTo(data, MAX_BYTES, m_addr);
			m_msgManager.addMsg("Me: " + std::string(data));
			m_msgManager.PrintMsg(true);
			if (!strcmp(data, "exit")) exit(EXIT_SUCCESS); //trust on guillermo
		}
	}
};

