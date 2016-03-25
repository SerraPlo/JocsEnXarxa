#include "Client.h"
#include <algorithm>

Client::Client(const std::string &serverAddress, const std::string &nick, UserData &uD) : m_nick(nick), m_userData(uD) {
	m_addr.setAddress(serverAddress);
	m_tcpSocket.Connect(m_addr);
	m_tcpSocket.NonBlocking(true);
}

static void Refresh(void) {
	system("cls");
	std::cout << "//////////////////////////////" << std::endl;
	std::cout << "\tWORD BATTLE" << std::endl;
	std::cout << "//////////////////////////////" << std::endl << std::endl;
}

inline void Client::SendMsg(KeyMsg key, const std::string& data) const {
	m_tcpSocket.Send((std::to_string(static_cast<int>(key)) + '_' + data).c_str());
}

bool Client::ProcessMsg(const std::string & data) {
	size_t pos = data.find_last_of('_');
	KeyMsg key = KeyMsg(atoi(data.substr(0, pos).c_str()));
	std::string msg = data.substr(pos + 1, data.size() - 1);

	switch (key) {
		// ------ BEGIN ------ //
		case KeyMsg::BEGIN: {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); //change console text color
			Refresh(); 
			SendMsg(KeyMsg::NICK, m_nick);
			m_ranking.resize(atoi(msg.c_str()));
		} return true;
		// ------ RANKING ------ //
		case KeyMsg::RANKING: {
			size_t first = 0, last;
			for (size_t i = 0; i < m_ranking.size(); ++i) {
				m_ranking[i].id = i;
				last = msg.find('#', first);
				m_ranking[i].nick = msg.substr(first, last);
				first = msg.find('#', last) + 1;
			}
			for (const auto &r : m_ranking) std::cout << r << std::endl;
			std::cout << std::endl;
		} return true;
		// ------- WORD ------- //
		case KeyMsg::WORD: {
			std::cout << "==> You have to copy: " << msg << std::endl;
		} return true;
		// ------ OKWORD ------ //
		case KeyMsg::OKWORD: {
			Refresh();
			msg == m_nick ?	std::cout << "It's correct! :)" << std::endl << std::endl : 
							std::cout << msg << " answered correctly, not you soz :(" << std::endl << std::endl;
		} return true;
		// ------ KOWORD ------ //
		case KeyMsg::KOWORD: {
			std::cout << "N00b... try again" << std::endl;
		} return true;
		// ------ SCORE ------ //
		case KeyMsg::SCORE: {
			std::cout << "------------------------------" << std::endl << "\tRANKING" << std::endl << "------------------------------" << std::endl;
			int newId = atoi(msg.c_str());
			++(*std::find_if(m_ranking.begin(), m_ranking.end(), [newId](const UserInfo &a) { return a.id == newId; })).score; //look for same player id
			std::sort(m_ranking.begin(), m_ranking.end(), [](const UserInfo &a, const UserInfo &b) { return a.score > b.score; }); //sort players by score
			for (const auto &r : m_ranking) std::cout << r << std::endl;
			std::cout << "------------------------------" << std::endl << std::endl;
		} return true;
		// ------- EXIT ------- //
		case KeyMsg::EXIT: {
			Refresh();
			std::cout << "==============================" << std::endl << "\tGAME OVER" << std::endl << "==============================" << std::endl << std::endl;
			std::sort(m_ranking.begin(), m_ranking.end(), [](const UserInfo &a, const UserInfo &b) { return a.score > b.score; });
			for (const auto &r : m_ranking) std::cout << r << std::endl;
			exit(EXIT_SUCCESS);
		} return true;
		// ---  DISCONNECT --- //
		case KeyMsg::DISCONNECT: {
			Refresh();
			std::cout << "Player " << msg << " has been disconnected" << std::endl;
			std::cout << "Current game avorted" << std::endl << std::endl;
			std::cout << "==============================" << std::endl << "\tGAME OVER" << std::endl << "==============================" << std::endl << std::endl;
			std::sort(m_ranking.begin(), m_ranking.end(), [](const UserInfo &a, const UserInfo &b) { return a.score > b.score; });
			for (const auto &r : m_ranking) std::cout << r << std::endl;
			exit(EXIT_SUCCESS); //provisional exit
		} return true;

		default: return false;
	}
}

void Client::CheckBegin(void) {
	std::cout << "Waiting for all players to be connected..." << std::endl;
	while (true) {
		char data[MAX_BYTES];
		if (!m_tcpSocket.Receive(data, MAX_BYTES)) continue;
		if (ProcessMsg(std::string(data))) break;
	}
}

void Client::GameLoop(void) {
	std::string word = "";
	while (true) {
		char msg[MAX_BYTES];
		if (m_userData.GetWord(word)) SendMsg(KeyMsg::WRITE, word);
		if (m_tcpSocket.Receive(msg, MAX_BYTES) > 0) ProcessMsg(msg);
	}
}

void Client::Run(void) {
	CheckBegin();
	GameLoop();
}

