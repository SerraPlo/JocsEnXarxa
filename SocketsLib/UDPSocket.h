#pragma once
#include "GenericSocket.h"

class UDPSocket : public GenericSocket
{
public:
	explicit UDPSocket();
	~UDPSocket() = default;

	int SendTo(const void* data, int lenData, SocketAddress& to);
	int ReceiveFrom(void* data, int lenData, SocketAddress& from);
};

