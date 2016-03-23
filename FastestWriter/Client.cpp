#include "Client.h"
#include <iostream>
#include <thread>

Client::Client(const char* serverAddress, const std::string &nick, UserData &uD) : m_nick(nick), m_data(uD) {
	m_addr.setAddress(serverAddress);
	m_tcpSocket.Connect(m_addr);
	m_tcpSocket.NonBlocking(true);
}

bool Client::ProcessMsg(const std::string & data) {
	auto pos = data.find_last_of('_');
	std::string key = data.substr(0, pos);
	std::string msg = data.substr(pos + 1, data.size() - 1);
	if (key == "BEGIN") {
		std::cout << "All players connected. Game begins." << std::endl;
		m_tcpSocket.Send((std::string("NICK_") + m_nick).c_str());
		return true;
	}
	else if (key == "WORD") {
		std::cout << "You have to copy: " << msg << std::endl;
		return true;
	}
	else if (key == "KOWORD") {
		std::cout << "N00b... try again" << std::endl;
		return true;
	}
	else if (key == "OKWORD") {
		if(msg==m_nick) std::cout << msg << "you win madafaka, congrats" << std::endl;
		else std::cout << msg << " wins madafaka, you not" << std::endl;
		return true;
	}
	else if (key == "SCORE") {
		std::cout << msg << std::endl;//falta ordenar
	}
	return false;
}

void Client::CheckBegin(void) {
	char data[MAX_BYTES];
	while (true) {
		if (!m_tcpSocket.Receive(data, MAX_BYTES)) continue;
		ProcessMsg(std::string(data));
	}
}

void Client::GameLoop(void) {
	char msg[MAX_BYTES];
	std::string word = "";
	while (true) {
		word = "QQREWAEERT";
		if(m_data.GetWord(word)) m_tcpSocket.Send((std::string("WRITE_") + word).c_str());
		std::cout << word << std::endl;
		if (m_tcpSocket.Receive(msg, MAX_BYTES) > 0) {
			ProcessMsg(msg);
			strcpy_s(msg, "");
		}
		//std::cout << "game loop" << std::endl;
	}
}

void Client::Run(void) {
	CheckBegin();
	GameLoop();
}

