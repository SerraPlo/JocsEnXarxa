#include "GenericSocket.h"
#include <iostream>
GenericSocket::GenericSocket(int type) : m_type(type) {
	m_socket = socket(AF_INET, type, IPPROTO_IP);
	if (m_socket == INVALID_SOCKET) SocketTools::ShowError("Socket could not be initialised.");
}

void GenericSocket::Bind(SocketAddress & address) {
	if (bind(m_socket, &address.getAddress(), sizeof(sockaddr)) == -1) SocketTools::ShowError("Address could not be binded to socket.");
}

GenericSocket::~GenericSocket() {
	//if (m_type == SOCK_STREAM) shutdown(m_socket, SD_BOTH);
	if (closesocket(m_socket) != NO_ERROR) SocketTools::ShowError("Socket could not be destroyed.");
}
