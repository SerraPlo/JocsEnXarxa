#include "Client.h"
#include <iostream>

Client::Client(const char* serverAddress, const std::string &nick) : m_nick(nick) {
	m_addr.setAddress(serverAddress);
	m_tcpSocket.Connect(m_addr);
	m_tcpSocket.NonBlocking(true);
}

bool Client::ProcessMsg(const std::string & data) {
	return false;
}

void Client::CheckBegin(void) const {
	char data[1300];
	while (true) {
		if (!m_tcpSocket.Receive(data, MAX_BYTES)) break;
		if (strcmp(data, "BEGIN")) {
			m_tcpSocket.Send((std::string("NICK_") + m_nick).c_str());
			break;
		}
	}
	std::cout << "All players connected. Game begins." << std::endl;
}

void Client::GameLoop(void) {
	std::string msg = "";
	while (true) {
		ProcessMsg(msg);
		std::cout << "game loop" << std::endl;
		break;
	}
}

void Client::Run(void) {
	CheckBegin();
	GameLoop();
}

