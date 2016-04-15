#pragma once
#include <vector>
#include <fstream>
#include "ClientProxy.h"

//////////////////////////
//	   LIST WORDS		//
//////////////////////////
class ListWords
{
	std::vector<string> m_words;
	size_t m_indexWord {0};
	void LoadFile() {
		std::ifstream wordsFile("words.txt");
		string line;
		if (wordsFile.is_open()) {
			while (getline(wordsFile, line)) m_words.push_back(line);
			wordsFile.close();
		}
	}
public:
	ListWords() { LoadFile(); };
	~ListWords() = default;

	string Current() { return m_words[m_indexWord]; };
	string Next() { return m_words[++m_indexWord]; };
	size_t CurrentIndex() const { return m_indexWord; };

	size_t Size() const { return m_words.size(); };
};

//////////////////////////
//	   MESSAGE KEYS		//
//////////////////////////
enum class KeyMsg {
	BEGIN = 1, RANKING, WORD, OKWORD, KOWORD, SCORE, EXIT, DISCONNECT, //client 
	NICK, WRITE //server 
};

//////////////////////////
//		 SERVER			//
//////////////////////////
class Server
{
	const int m_numPlayers;									//max players to be connected
	TCPSocket m_dispatcher;									//main server's socket
	ListWords m_wordsList;									//list of words to be displayed for the game
	std::vector<ClientProxy> m_clientList;					//list of clients with its nickname, score and socket reference

	void SendToAll(KeyMsg key, const string &data);	//send message to all clients
	void SendToAll(KeyMsg key);
	inline void SendTo(int id, KeyMsg key);
	void SendRanking(void);
	inline void SendScore(int id);
	bool ProcessMsg(size_t id, const string &data);		//operate according to message key
	void InitConnection(void);								//initialize connection for all the players
	void SetNicks(void);									//store nicknames in list of clients
	void GameLoop(void);									//main server's game loop
public:
	explicit Server(const char* bindAddress, int numPlayers);
	~Server() = default;

	void Run(void);
};

