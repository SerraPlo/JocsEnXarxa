#include "TCPSocket.h"

TCPSocket::TCPSocket() : GenericSocket(SOCK_STREAM) {}

TCPSocket::TCPSocket(SOCKET a) : GenericSocket(SOCK_STREAM) {
	m_socket = a;
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
	if (connect(m_socket, addr.getPtrAddress(), sizeof(sockaddr)) == SOCKET_ERROR)
		SocketTools::ThrowError("TCPSocket: cannot connect to addr.");
}

void TCPSocket::Send(const void * data) const {
	if (send(m_socket, static_cast<const char*>(data), MAX_BYTES, 0) == SOCKET_ERROR) {
		SocketTools::ThrowError("TCPSocket: can't send data.");
	}
}

int TCPSocket::Receive(void * data, int lenData) const {
	int bytesReceived = recv(m_socket, static_cast<char*>(data), lenData, 0);
	/*if (bytesReceived == SOCKET_ERROR)
		SocketTools::ThrowError("TCPSocket: error receiving data.");*/
	/*else if (bytesReceived == 0)
		std::cout << "TCPSocket: Connection closed." << std::endl;*/
	return bytesReceived;
}
