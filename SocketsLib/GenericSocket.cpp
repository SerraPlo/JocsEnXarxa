#include "GenericSocket.h"

GenericSocket::GenericSocket(int type) : m_type(type) {
	m_socket = socket(AF_INET, type, IPPROTO_IP);
	if (m_socket == INVALID_SOCKET) 
		SocketTools::ThrowError("Socket could not be initialised.");
}

void GenericSocket::Bind(SocketAddress & address) const {
	if (bind(m_socket, address.getSockaddrAddress(), sizeof(sockaddr)) == SOCKET_ERROR)
		SocketTools::ThrowError("Address could not be binded to socket.");
}

// TODO: cross-paltform
void GenericSocket::NonBlocking(bool isNonBlocking) const {
	u_long mode = isNonBlocking ? 1 : 0;
	if (ioctlsocket(m_socket, FIONBIO, &mode) == SOCKET_ERROR) 
		SocketTools::ThrowError("Socket could not be changed to non-blocking mode.");
}

GenericSocket::~GenericSocket() {
	if (closesocket(m_socket) != NO_ERROR)
		SocketTools::ThrowError("Socket could not be destroyed.");
}
