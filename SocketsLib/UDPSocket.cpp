#include "UDPSocket.h"

UDPSocket::UDPSocket() : GenericSocket(SOCK_DGRAM) {}

int UDPSocket::SendTo(const void* data, int lenData, SocketAddress& to) const {
	auto bytesSent = sendto(m_socket, static_cast<const char*>(data), lenData, 0, to.getSockaddrAddress(), sizeof(sockaddr));
	if (bytesSent == SOCKET_ERROR) SocketTools::ThrowError("Data could not be sent.");
	return bytesSent;
}

int UDPSocket::ReceiveFrom(void* data, int lenData, SocketAddress & from) const {
	sockaddr tempAddr;
	int addrSize = sizeof(sockaddr);
	auto bytesReceived = recvfrom(m_socket, static_cast<char*>(data), lenData, 0, &tempAddr, &addrSize);
	from.setAddress(tempAddr);
	if (bytesReceived == SOCKET_ERROR && !m_isNonBlocking) SocketTools::ThrowError("Data could not be received.");
	return bytesReceived;
}
