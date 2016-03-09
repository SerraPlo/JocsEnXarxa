#pragma once
#include "SocketAddress.h"

#define MAX_BYTES 1300

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
	void NonBlocking(bool isNonBlocking) const;

	GenericSocket& operator=(const GenericSocket& other) = default;
};