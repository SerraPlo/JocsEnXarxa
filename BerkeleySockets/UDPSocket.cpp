#include "UDPSocket.h"
#include <iostream>

UDPSocket::UDPSocket() : GenericSocket(SOCK_DGRAM)
{}

int UDPSocket::SendTo(const void* data, int lenData, SocketAddress& to) {
	int st = sendto(m_socket, (char*)(data), lenData, 0, &to.getAddress(), sizeof(sockaddr_in));
	if (st == -1) SocketTools::ShowError("Data could not be sent.");
	return st;
}

int UDPSocket::ReceiveFrom(void* data, int lenData, SocketAddress & from) {
	sockaddr sa;
	int sadr = sizeof(sockaddr);
	int rf = recvfrom(m_socket, (char*)(data), lenData, 0, &sa, &sadr);
	from.setAddress(sa);
	if (rf == -1) SocketTools::ShowError("Data could not be received.");
	return rf;
}
