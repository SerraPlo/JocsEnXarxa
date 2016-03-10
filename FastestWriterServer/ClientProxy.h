#pragma once
#include <TCPSocket.h>
#include <string>
#include <memory>

class ClientProxy
{
	std::string m_nick = ""; //client's nickname
	int m_score = 0; //game's client score
	std::shared_ptr<TCPSocket> m_tcpSocket; //reference to acceptance socket created from dispatcher
public:
	explicit ClientProxy() = default;
	explicit ClientProxy(const std::shared_ptr<TCPSocket> &tcp);
	~ClientProxy() = default;

	const std::string& GetNick() const;
	void SetNick(const std::string &nick);

	int GetScore() const;
	void SetScore(int score);

	void Send(const std::string &msg) const; //send message from socket
	int Receive(std::string& msg) const; //receive message from socket

	bool CheckNick(const std::string &str) const;
};

