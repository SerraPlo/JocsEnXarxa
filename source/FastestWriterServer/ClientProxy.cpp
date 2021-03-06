#include "ClientProxy.h"

ClientProxy::ClientProxy(const std::shared_ptr<TCPSocket> &tcp) :
	m_tcpSocket(std::shared_ptr<TCPSocket>(tcp))
{}

const string& ClientProxy::GetNick() const {
	return m_nick;
}

void ClientProxy::SetNick(const string& str) {
	m_nick = str;
}

int ClientProxy::GetScore() const {
	return m_score;
}

void ClientProxy::AddScore() {
	++m_score;
}

void ClientProxy::Send(const string &msg) const {
	m_tcpSocket->Send(msg.c_str());
}

int ClientProxy::Receive(string& msg) const {
	char data[MAX_BYTES];
	int bytesReceived = m_tcpSocket->Receive(data, MAX_BYTES);
	msg = data;
	return bytesReceived;
}

bool ClientProxy::EmptyNick() const {
	return ("" == m_nick);
}
