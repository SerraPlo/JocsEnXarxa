#include "UDPSocket.h"

UDPSocket::UDPSocket() : GenericSocket(SOCK_DGRAM) {}

int UDPSocket::SendTo(const void* data, int lenData, SocketAddress& to) const {
	auto bytesSent = sendto(m_socket, static_cast<const char*>(data), lenData, 0, to.getSockaddrAddress(), sizeof(sockaddr_in));
	if (bytesSent == SOCKET_ERROR) SocketTools::ThrowError("Data could not be sent.");
	return bytesSent;
}

int UDPSocket::ReceiveFrom(void* data, int lenData, SocketAddress & from) const {
	memset(data, NULL, MAX_BYTES);

	sockaddr tempAddr;
	int addrSize = sizeof(sockaddr_in);
	auto cData = static_cast<char*>(data);
	auto bytesReceived = recvfrom(m_socket, cData, lenData, 0, &tempAddr, &addrSize);
	from.setAddress(tempAddr);

	if (bytesReceived == SOCKET_ERROR && !m_isNonBlocking) SocketTools::ThrowError("Data could not be received.");

	if (bytesReceived < MAX_BYTES && bytesReceived > 0) cData[bytesReceived] = '\0';
	else if (bytesReceived == MAX_BYTES)
		for (int i = 0; i < MAX_BYTES; ++i) if (cData[i] == NULL) { bytesReceived = i; cData[i] = '\0'; break; }

	return bytesReceived;
}
