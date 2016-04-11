#include "Client.h"
#include <algorithm>
#include <iostream>
using std::cout;
using std::endl;
using std::string;

//CLIENT STATIC VARIABLES FOR THREADS
static UserData globalUserData = {};
bool globalBeginGame = false;

#define REFRESH() system("cls"); \
cout << "//////////////////////////////" << endl; \
cout << "\tWORD BATTLE" << endl; \
cout << "//////////////////////////////" << endl << endl;

#define PRINT_RANKING() for (const auto &r : m_ranking) cout << r << endl;
#define SORT_RANKING() std::sort(m_ranking.begin(), m_ranking.end(), [](const UserInfo &a, const UserInfo &b) { return a.score > b.score; });

Client::Client(const string &serverAddress, const string &nick) : m_nick(nick) {
	SocketAddress m_addr; //store server address to connect
	m_addr.setAddress(serverAddress);
	m_tcpSocket.Connect(m_addr);
	m_tcpSocket.NonBlocking(true);
	//m_tcpSocket.DisableDelay();
}

inline void Client::SendMsg(KeyMsg key, const string& data) const {
	m_tcpSocket.Send((std::to_string(static_cast<int>(key)) + '_' + data).c_str());
}

bool Client::ProcessMsg(const string & data) {
	size_t pos = data.find_last_of('_');
	KeyMsg key = KeyMsg(atoi(data.substr(0, pos).c_str()));
	string msg = data.substr(pos + 1, data.size() - 1);

	switch (key) {
		// ------ BEGIN ------ //
		case KeyMsg::BEGIN: {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); //change console text color
			REFRESH();
			SendMsg(KeyMsg::NICK, m_nick);
			m_ranking.resize(atoi(msg.c_str()));
		} return true;
		// ------ RANKING ------ //
		case KeyMsg::RANKING: {
			size_t first = 0, last, p;
			for (size_t i = 0; i < m_ranking.size()-1; ++i) {
				m_ranking[i].id = i;
				p = msg.find_first_of('#', first);
				last = p - first;
				m_ranking[i].nick = msg.substr(first, last);
				first = p + 1;
			}
			int lastID = m_ranking.size() - 1;
			m_ranking[lastID].id = lastID;
			m_ranking[lastID].nick = msg.substr(first, msg.size()-1);
			PRINT_RANKING();
			cout << endl;
		} return true;
		// ------- WORD ------- //
		case KeyMsg::WORD: {
			cout << "==> You have to copy: " << msg << endl;
		} return true;
		// ------ OKWORD ------ //
		case KeyMsg::OKWORD: {
			REFRESH();
			msg == m_nick ?	cout << "It's correct! :)" << endl << endl : 
							cout << msg << " answered correctly, not you soz :(" << endl << endl;
		} return true;
		// ------ KOWORD ------ //
		case KeyMsg::KOWORD: {
			cout << "N00b... try again" << endl;
		} return true;
		// ------ SCORE ------ //
		case KeyMsg::SCORE: {
			cout << "------------------------------" << endl << "\tRANKING" << endl << "------------------------------" << endl;
			int newId = atoi(msg.c_str());
			++(*std::find_if(m_ranking.begin(), m_ranking.end(), [newId](const UserInfo &a) { return a.id == newId; })).score; //look for same player id
			SORT_RANKING(); PRINT_RANKING();
			cout << "------------------------------" << endl << endl;
		} return true;
		// ------- EXIT ------- //
		case KeyMsg::EXIT: {
			REFRESH();
			cout << "==============================" << endl << "\tGAME OVER" << endl << "==============================" << endl << endl;
			std::sort(m_ranking.begin(), m_ranking.end(), [](const UserInfo &a, const UserInfo &b) { return a.score > b.score; });
			SORT_RANKING(); PRINT_RANKING();
			exit(EXIT_SUCCESS);
		} return true;
		// ---  DISCONNECT --- //
		case KeyMsg::DISCONNECT: {
			REFRESH();
			cout << "Player " << msg << " has been disconnected" << endl << "Current game avorted" << endl << endl;
			cout << "==============================" << endl << "\tGAME OVER" << endl << "==============================" << endl << endl;
			SORT_RANKING(); PRINT_RANKING();
			exit(EXIT_SUCCESS); //provisional exit
		} return true;

		default: return false;
	}
}

static void IgnoreInput() {
	std::mutex mtx;
	string input;
	while (!globalBeginGame) {
		mtx.lock();
		getline(std::cin, input);
		mtx.unlock();
	}
	globalUserData.SetWord(input);
}

void Client::CheckBegin(void) {
	cout << "Waiting for all players to be connected..." << endl;

	std::thread ignoreInputThread(IgnoreInput);
	ignoreInputThread.detach();

	char data[MAX_BYTES];
	while (!globalBeginGame) {
		if (!m_tcpSocket.Receive(data, MAX_BYTES)) continue;
		if (ProcessMsg(string(data))) globalBeginGame = true;
	}
}

static void GetInput() {
	string input;
	while (getline(std::cin, input)) {
		globalUserData.SetWord(input);
	}
}

void Client::GameLoop(void) {
	string word = "";

	std::thread getInputThread(GetInput);
	getInputThread.detach();

	char msg[MAX_BYTES];
	while (true) {
		if (globalUserData.GetWord(word)) SendMsg(KeyMsg::WRITE, word);
		if (m_tcpSocket.Receive(msg, MAX_BYTES) > 0) ProcessMsg(msg);
	}
}

void Client::Run(void) {
	CheckBegin();
	GameLoop();
}

#undef REFRESH
#undef PRINT_RANKING
#undef SORT_RANKING