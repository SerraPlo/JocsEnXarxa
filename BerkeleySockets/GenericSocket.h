#pragma once
#include "SocketAddress.h"

class GenericSocket
{
protected:
	SOCKET m_socket;
	int m_type;
public:
	GenericSocket(int type);
	virtual ~GenericSocket();
	void Bind(SocketAddress& address);
	virtual int SendTo(const void* data, int lenData, SocketAddress& to) = 0;
	virtual int ReceiveFrom(void* data, int lenData, SocketAddress& from) = 0;
};