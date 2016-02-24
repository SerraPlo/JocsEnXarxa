#pragma once
#include "GenericSocket.h"

class UDPSocket : public GenericSocket
{
public:
	UDPSocket();
	~UDPSocket() = default;

	int SendTo(const void* data, int lenData, SocketAddress& to) override;
	int ReceiveFrom(void* data, int lenData, SocketAddress& from) override;
};

