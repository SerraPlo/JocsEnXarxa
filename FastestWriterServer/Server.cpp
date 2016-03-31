#include "Server.h"
#include <iostream>

Server::Server(const char* bindAddress, int numPlayers) : m_numPlayers(numPlayers){
	SocketAddress m_addr; //main address to be binded to socket
	m_addr.setAddress(bindAddress);
	m_dispatcher.Bind(m_addr);
}

void Server::SendToAll(KeyMsg key, const std::string &data) {
	for (auto client : m_clientList) client.Send((std::to_string(static_cast<int>(key)) + '_' + data).c_str());
}

void Server::SendToAll(KeyMsg key) {
	for (auto client : m_clientList) client.Send(std::to_string(static_cast<int>(key)));
}

inline void Server::SendTo(int id, KeyMsg key) {
	m_clientList[id].Send(std::to_string(static_cast<int>(key)));
}

void Server::SendRanking(void) {
	std::string line = m_clientList[0].GetNick();
	for (size_t i = 1; i < m_clientList.size(); ++i) line += '#' + m_clientList[i].GetNick();
	SendToAll(KeyMsg::RANKING, line);
}

inline void Server::SendScore(int id) {
	m_clientList[id].AddScore();
	SendToAll(KeyMsg::SCORE, std::to_string(id));
}

bool Server::ProcessMsg(size_t id, const std::string &data) {
	auto pos = data.find_last_of('_');
	KeyMsg key = KeyMsg(atoi(data.substr(0, pos).c_str()));
	std::string msg = data.substr(pos+1, data.size()-1);

	switch (key) {
		case KeyMsg::NICK: {
			m_clientList[id].SetNick(msg);
		} return true;
		case KeyMsg::WRITE: {
			if (msg == "exit") { SendToAll(KeyMsg::DISCONNECT, m_clientList[id].GetNick()); return true; }
			if (msg == m_wordsList.Current()) {
				SendToAll(KeyMsg::OKWORD, m_clientList[id].GetNick());
				SendScore(id);
				if (m_wordsList.CurrentIndex() >= m_wordsList.Size()-1) {
					std::cout << "This is the end of this magnificent game yo!" << std::endl;
					SendToAll(KeyMsg::EXIT);
					system("pause");
					exit(EXIT_SUCCESS);
				}
				SendToAll(KeyMsg::WORD, m_wordsList.Next());//paraula si hi ha guanyaaoisadsf
				return true;
			}
			SendTo(id, KeyMsg::KOWORD);
		} return true;
		default: return false;
	}
}

void Server::InitConnection(void) {
	m_dispatcher.Listen(m_numPlayers); //establish connection for m_numPlayers clients
	for (int i = 0; i < m_numPlayers; ++i) {
		SocketAddress from;
		printf("Waiting for player %d\n", i + 1);
		std::shared_ptr<TCPSocket> tempSocket = m_dispatcher.Accept(from); //block until client is connected
		tempSocket->NonBlocking(true); //set non-blocking socket enabled
		m_clientList.emplace_back(tempSocket); //add new player to the list
		printf("Connected to player %d\n", i + 1);
	}
	SendToAll(KeyMsg::BEGIN, std::to_string(m_numPlayers));
}

void Server::SetNicks(void) {
	std::string tempData = "";
	int incompletes = m_numPlayers; //remaining players
	while (incompletes > 0) {
		for (size_t i = 0; i < m_clientList.size(); ++i) {
			if (m_clientList[i].EmptyNick()) { //if nick not set, proceed for check
				if (m_clientList[i].Receive(tempData) <= 0) continue; //if == 0, nothing received, keep looping
				if (ProcessMsg(i, tempData)) --incompletes; //if message is of type NICK, set nick
			}
		}
	}
	std::cout << "All players connected. Game begins." << std::endl;
	SendRanking();
}

void Server::GameLoop(void) {
	std::string tempData = "";
	SendToAll(KeyMsg::WORD, m_wordsList.Current());
	while (true) {
		for (size_t i = 0; i < m_clientList.size(); ++i)
			if (m_clientList[i].Receive(tempData) > 0) ProcessMsg(i, tempData);
	}
}

void Server::Run(void) {
	InitConnection();
	SetNicks();
	GameLoop();
}
