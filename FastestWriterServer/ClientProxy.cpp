#include "ClientProxy.h"

ClientProxy::ClientProxy(const std::shared_ptr<TCPSocket> &tcp) :
	m_tcpSocket(std::shared_ptr<TCPSocket>(tcp))
{}

const std::string& ClientProxy::GetNick() const {
	return m_nick;
}

void ClientProxy::SetNick(const std::string& str) {
	m_nick = str;
}

int ClientProxy::GetScore() const {
	return m_score;
}

void ClientProxy::SetScore(int score) {
	m_score = score;
}

void ClientProxy::Send(const std::string &msg) const {
	m_tcpSocket->Send(msg.c_str());
}

int ClientProxy::Receive(std::string& msg) const {
	char data[MAX_BYTES];
	int bytesReceived = m_tcpSocket->Receive(data, MAX_BYTES);
	if (bytesReceived < MAX_BYTES) data[bytesReceived] = '\0';
	msg = data;
	return bytesReceived;
}

bool ClientProxy::CheckNick(const std::string &str) const {
	return (str == m_nick);
}