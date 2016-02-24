#pragma once
#include "SocketTools.h"
#include <ws2tcpip.h>

class SocketAddress
{
	sockaddr_in m_address;
public:
	SocketAddress() = default;
	SocketAddress(const std::string &inString);
	SocketAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint16_t port);
	SocketAddress(SocketAddress &a);
	~SocketAddress() = default;

	int setAddress(const std::string &inString);
	void setAddress(sockaddr a);
	sockaddr getAddress();

	friend std::ostream& operator<<(std::ostream& os, const SocketAddress &sa);
	bool operator==(SocketAddress& sa);
};

