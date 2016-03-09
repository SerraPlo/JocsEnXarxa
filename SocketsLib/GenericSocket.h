#pragma once
#include "SocketAddress.h"

class GenericSocket
{
protected:
	SOCKET m_socket;
	int m_type;
public:
	explicit GenericSocket(int type);
	GenericSocket(const GenericSocket& other) = default;
	virtual ~GenericSocket();

	void Bind(SocketAddress& address) const;
	//virtual int SendTo(const void* data, int lenData, SocketAddress& to) = 0;
	//virtual int ReceiveFrom(void* data, int lenData, SocketAddress& from) = 0;

	GenericSocket& operator=(const GenericSocket& other) = default;
};