#pragma once
#include <TCPSocket.h>
#include <string>
#include <memory>
using std::string;

class ClientProxy
{
	string m_nick {""}; //client's nickname
	int m_score {0}; //game's client score
	std::shared_ptr<TCPSocket> m_tcpSocket; //reference to acceptance socket created from dispatcher
public:
	explicit ClientProxy(string n = "", int s = 0) {
		m_nick = n;
		m_score = s;
	}
	explicit ClientProxy(const std::shared_ptr<TCPSocket> &tcp);
	~ClientProxy() = default;

	const string& GetNick() const;
	void SetNick(const string &nick);

	int GetScore() const;
	void AddScore();

	void Send(const string &msg) const; //send message from socket
	int Receive(string& msg) const; //receive message from socket

	bool EmptyNick() const;
};

