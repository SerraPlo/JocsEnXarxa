#pragma once
#include <TCPSocket.h>
#include <string>
#include <memory>

class ClientProxy
{
	std::string m_nick = "";
	int m_score = 0;
	std::shared_ptr<TCPSocket> m_tcpSocket;
public:
	explicit ClientProxy() = default;
	explicit ClientProxy(const std::shared_ptr<TCPSocket> &tcp);
	~ClientProxy() = default;

	const std::string& GetNick() const;
	void SetNick(const std::string &nick);

	int GetScore() const;
	void SetScore(int score);

	void Send(const std::string &msg) const;
	int Receive(std::string& msg) const;

	bool CheckNick(const std::string &str) const;
};

