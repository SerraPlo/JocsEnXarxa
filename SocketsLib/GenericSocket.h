#pragma once
#include "SocketAddress.h"

#define MAX_DATA 1300

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
	void NonBlocking(bool isNonBlocking);

	GenericSocket& operator=(const GenericSocket& other) = default;
};