#include "TCPSocket.h"
#include <iostream>

TCPSocket::TCPSocket() : GenericSocket(SOCK_STREAM) {}

TCPSocket::TCPSocket(SOCKET a) : GenericSocket(SOCK_STREAM) {
	m_socket = a;
}

TCPSocket::~TCPSocket(){
	if (shutdown(m_socket, SD_BOTH) == SOCKET_ERROR)
		SocketTools::ThrowError("Socket: problem with shutdown.");
}

void TCPSocket::Listen(int backlog){
	if (listen(m_socket, backlog) == SOCKET_ERROR)
		SocketTools::ThrowError("Socket: problem with shutdown.");
}

std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress & from){
	sockaddr sa;
	int sizeSa = sizeof(sockaddr);
	SOCKET newSocket = accept(m_socket, &sa, &sizeSa);
	from.setAddress(sa);
	return std::shared_ptr<TCPSocket>(new TCPSocket(newSocket));
}

void TCPSocket::Connect(SocketAddress & addr) {
	if (connect(m_socket,addr.getPtrAddress(),sizeof(addr)) == SOCKET_ERROR)
		SocketTools::ThrowError("Socket: problem with connect.");
}

void TCPSocket::Send(const void * data){
	if (send(m_socket, (const char*)data, MAX_DATA, 0) == SOCKET_ERROR) {
		SocketTools::ThrowError("Socket: can't send.");
	}
}

void TCPSocket::Receive(void * data, int lenData){
	int bytesReceived = recv(m_socket, (char*)data, lenData, 0);
	if (bytesReceived == SOCKET_ERROR)
		SocketTools::ThrowError("Socket: problem with receive.");
	else if (bytesReceived == 0)
		std::cout << "Conexion cerrada" << std::endl;
}
