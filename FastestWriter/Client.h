#pragma once

#include <iostream>
#include <string>
#include <mutex>

//////////////////////////
//		USER DATA		//
//////////////////////////
class UserData
{
	std::string m_word;
	std::mutex m_blocker;
	bool m_sentToServer = true;
public:
	explicit UserData() = default;
	~UserData() = default;
	void SetWord(const std::string &str) {
		m_blocker.lock();
		m_word = str;
		m_sentToServer = false;
		m_blocker.unlock();
	}
	bool GetWord(std::string &str) {
		str = m_word;
		if (m_sentToServer) return false;
		m_sentToServer = true;
		return true;
	}
};

//////////////////////////
//		USER INPUT		//
//////////////////////////
class UserInput
{
	UserData &m_data;
public:
	explicit UserInput(UserData &a) : m_data(a) {}
	~UserInput() = default;
	void operator()() const {
		std::string input;
		while (true) std::getline(std::cin, input), m_data.SetWord(input);
	}
};

//////////////////////////
//		USER INFO		//
//////////////////////////
struct UserInfo {
	int id = 0;
	int score = 0;
	std::string nick = "";
	friend std::ostream& operator<<(std::ostream& os, const UserInfo &ui) { return os << "nick: " << ui.nick << "\tscore: " << ui.score; }
};

//////////////////////////
//	   MESSAGE KEYS		//
//////////////////////////
enum class KeyMsg {
	BEGIN = 1, RANKING, WORD, OKWORD, KOWORD, SCORE, EXIT, DISCONNECT, //client 
	NICK, WRITE //server 
};

#include <TCPSocket.h>
#include <vector>

//////////////////////////
//		 CLIENT			//
//////////////////////////
class Client 
{
	SocketAddress m_addr;											//store server address to connect
	TCPSocket m_tcpSocket;											//main client socket
	const std::string m_nick;										//client's nickname
	UserData &m_userData;											//user data reference to main data
	std::vector<UserInfo> m_ranking;								//ranking of players

	inline void SendMsg(KeyMsg key, const std::string& data) const;	//send a message to the server
	bool ProcessMsg(const std::string &data);						//operate according to message key
	void CheckBegin(void);											//check if game begins, then send the nickname
	void GameLoop(void);											//main player's game loop
public:
	explicit Client(const std::string &serverAddress, const std::string &nick, UserData &uD);
	~Client() = default;

	void Run(void);
};

