#include "GenericSocket.h"

GenericSocket::GenericSocket(int type) : m_type(type) {
	m_socket = socket(AF_INET, type, IPPROTO_IP);
	if (m_socket == INVALID_SOCKET) 
		SocketTools::ThrowError("Socket could not be initialised.");
}

void GenericSocket::Bind(SocketAddress & address) const {
	if (bind(m_socket, address.getPtrAddress(), sizeof(sockaddr)) == -1) 
		SocketTools::ThrowError("Address could not be binded to socket.");
}

GenericSocket::~GenericSocket() {
	if (closesocket(m_socket) != NO_ERROR) 
		SocketTools::ThrowError("Socket could not be destroyed.");
}
