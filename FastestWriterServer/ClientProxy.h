#pragma once
#include <string>
#include <memory>
#include <TCPSocket.h>

class ClientProxy
{
	std::string m_nick = "";
	int m_score = 0;
	std::shared_ptr<TCPSocket> m_tcpSocket;
public:
	ClientProxy() = default;
	ClientProxy(const std::shared_ptr<TCPSocket> &tcp) :
		m_tcpSocket(std::shared_ptr<TCPSocket>(tcp))
	{}
	~ClientProxy() = default;

	inline void setNick(const std::string& str) {
		m_nick = str;
	}

	inline std::shared_ptr<TCPSocket> getSocket() {
		return m_tcpSocket;
	}
	inline bool checkNick(const std::string &str) {
		return (str == m_nick);
	}
};

