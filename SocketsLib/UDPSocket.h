#pragma once
#include "GenericSocket.h"

class UDPSocket : public GenericSocket
{
public:
	explicit UDPSocket();
	UDPSocket(const UDPSocket& other) = default;
	~UDPSocket() = default;

	int SendTo(const void* data, int lenData, SocketAddress& to) const;
	int ReceiveFrom(void* data, int lenData, SocketAddress& from) const;

	UDPSocket& operator=(const UDPSocket& other) = default;
};

