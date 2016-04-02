#include "TCPSocket.h"
#include <iostream>

TCPSocket::TCPSocket() : GenericSocket(SOCK_STREAM) {}

TCPSocket::TCPSocket(SOCKET s) : GenericSocket(SOCK_STREAM) {
	m_socket = s;
}

TCPSocket::~TCPSocket(){
	if (shutdown(m_socket, SD_BOTH) == SOCKET_ERROR)
		SocketTools::ThrowError("TCPSocket: error shuting down.");
}

void TCPSocket::Listen(int backlog) const {
	if (listen(m_socket, backlog) == SOCKET_ERROR)
		SocketTools::ThrowError("TCPSocket: error while listening from backlog.");
}

std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress & from) const {
	sockaddr sa;
	int sizeSa = sizeof(sockaddr);
	SOCKET newSocket = accept(m_socket, &sa, &sizeSa);
	from.setAddress(sa);
	return std::make_shared<TCPSocket>(newSocket);
}

void TCPSocket::Connect(SocketAddress & addr) const {
	if (connect(m_socket, addr.getSockaddrAddress(), sizeof(sockaddr)) == SOCKET_ERROR)
		SocketTools::ThrowError("TCPSocket: cannot connect to addr.");
}

void TCPSocket::Send(const void * data) const {
	const char* cData = static_cast<const char*>(data);
	if (send(m_socket, cData, MAX_BYTES, 0) == SOCKET_ERROR)
		SocketTools::ThrowError("TCPSocket: can't send data.");
}

int TCPSocket::Receive(void * data, int lenData) const {
	memset(data, NULL, MAX_BYTES);

	auto cData = static_cast<char*>(data);
	int bytesReceived = recv(m_socket, cData, lenData, 0); //blocks execution until data arrives if blocking socket mode
	
	if (bytesReceived == SOCKET_ERROR && !m_isNonBlocking) SocketTools::ThrowError("TCPSocket: error receiving data.");
	else if (bytesReceived == 0) std::cout << "TCPSocket: Connection closed." << std::endl, exit(EXIT_SUCCESS);

	if (bytesReceived == MAX_BYTES)
		for (int i = 0; i < MAX_BYTES; ++i) 
			if (cData[i] == NULL) { bytesReceived = i; cData[i] = '\0'; break; }
	
	return bytesReceived;
}
