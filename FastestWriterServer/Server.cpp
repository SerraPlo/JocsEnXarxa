#include "Server.h"
#include <iostream>

Server::Server(const char* bindAddress, int numPlayers) : m_numPlayers(numPlayers){
	m_addr.setAddress(bindAddress);
	m_dispatcher.Bind(m_addr);
	m_dispatcher.Listen(numPlayers);
}

void Server::SendToAll(const std::string &data) {
	for (auto client : m_clientList)
		client.Send(data);
}
void Server::SendTo(int id, const std::string &data) {
	m_clientList[id].Send(data);
}
void Server::SendRanking() {
	std::string line = "SCORE_";
	for (size_t i = 0; i < m_clientList.size(); ++i) {
		line += m_clientList[i].GetNick() + "#" + std::to_string(m_clientList[i].GetScore());
	}
	SendToAll(line);
}

bool Server::ProcessMsg(int id, const std::string &data) {
	auto pos = data.find_last_of('_');
	std::string key = data.substr(0, pos);
	std::string msg = data.substr(pos+1, data.size()-1);
	if (key == "NICK") {
		m_clientList[id].SetNick(msg);
		return true;
	}
	if (key == "WRITE") {
		if (!strcmp(msg.c_str(), m_wordsList.Current().c_str())) {
			SendToAll("OKWORD_" + m_clientList[id].GetNick());
			waitingWiner = true;
			SendRanking();
			m_wordsList.Next();
			return true;
		}
		else SendTo(id, "KOWORD");
	}
	return false;
}

void Server::InitConnection(void) {
	m_dispatcher.Listen(m_numPlayers); //establish connection for m_numPlayers clients
	SocketAddress from;
	for (int i = 0; i < m_numPlayers; ++i) {
		printf("Waiting for player %d\n", i + 1);
		std::shared_ptr<TCPSocket> tempSocket = m_dispatcher.Accept(from); //block until client is connected
		tempSocket->NonBlocking(true); //set non-blocking socket enabled
		m_clientList.emplace_back(tempSocket); //add new player to the list
		printf("Connected to player %d\n", i + 1);
	}
	SendToAll("BEGIN");
}

void Server::SetNicks(void) {
	std::string tempData = "";
	int incompletes = m_numPlayers; //remaining players
	while (incompletes > 0) {
		for (size_t i = 0; i < m_clientList.size(); ++i) {
			if (m_clientList[i].CheckNick("")) { //if nick not set, proceed for check
				if (!m_clientList[i].Receive(tempData)) continue; //if == 0, nothing received, keep looping
				if (ProcessMsg(i, tempData)) { //if message is of type NICK, set nick
					m_clientList[i].SetNick(tempData);
					--incompletes;
				}
			}
		}
	}
	
	std::cout << "All players connected. Game begins." << std::endl;
}

void Server::GameLoop(void) {
	waitingWiner = false;
	std::string tempData = "";
	while (true) {
		if (!waitingWiner) {
			SendToAll("WORD_" + m_wordsList.Current());//paraula si hi ha guanyaaoisadsf
			waitingWiner = true;
		}
		else {
			for (size_t i = 0; i < m_clientList.size(); ++i) {
				if (m_clientList[i].Receive(tempData)>0) {
					if (ProcessMsg(i, tempData)) {
						
					}
				}
			}
		}
	}
}

void Server::Run(void) {
	InitConnection();
	SetNicks();
	GameLoop();
}
