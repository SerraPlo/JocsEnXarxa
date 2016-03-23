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
		std::cout << "===================================" << std::endl << std::endl;
		m_tcpSocket.Send((std::string("NICK_") + m_nick).c_str());
		return true;
	}
	else if (key == "WORD") {
		std::cout << "==> You have to copy: " << msg << std::endl;
		return true;
	}
	else if (key == "OKWORD") {
		if (msg == m_nick) std::cout <<"CONGRATULATIONS "<<msg<<" YOU WIN" << std::endl;
		else std::cout << msg << " wins, you not soz :(" << std::endl;
		return true;
	}
	else if (key == "KOWORD") {
		std::cout << "N00b... try again" << std::endl;
		return true;
	}
	else if (key == "SCORE") {
		std::cout << "===================================" << std::endl;
		std::cout << msg << std::endl;//falta ordenar
		std::cout << "===================================" << std::endl;

	}
	else if (key == "EXIT") {
		std::cout << "GAME OVER" << std::endl;
		exit(EXIT_SUCCESS);
	}
	return false;
}

void Client::CheckBegin(void) {
	char data[MAX_BYTES];
	while (true) {
		if (!m_tcpSocket.Receive(data, MAX_BYTES)) continue;
		ProcessMsg(std::string(data));
		break;
	}
}

void Client::GameLoop(void) {
	char msg[MAX_BYTES];
	std::string word = "";
	while (true) { 
		if(m_data.GetWord(word)) m_tcpSocket.Send((std::string("WRITE_") + word).c_str());
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

