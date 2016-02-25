#pragma once
#include "SocketTools.h"

class SocketAddress
{
	sockaddr_in m_address;
public:
	explicit SocketAddress() = default;
	explicit SocketAddress(const std::string &inString);
	explicit SocketAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint16_t port);
	explicit SocketAddress(SocketAddress &a);
	~SocketAddress() = default;

	int setAddress(const std::string &inString);
	void setAddress(const sockaddr &a);
	sockaddr* getPtrAddress();

	friend std::ostream& operator<<(std::ostream& os, const SocketAddress &sa);
	bool operator==(SocketAddress& sa) const;
};

