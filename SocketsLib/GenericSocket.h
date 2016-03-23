#pragma once
#include "SocketAddress.h"

#ifndef _WIN32
	typedef int SOCKET;
	#define INVALID_SOCKET  (SOCKET)(~0)
	#define SOCKET_ERROR            (-1)
	#define NO_ERROR				(0L)
	#define closesocket(sockdep) close(sockdep)
	#define SD_BOTH					(0x02)
#endif

#define MAX_BYTES 1300 // max bytes of data to be sent/received

class GenericSocket
{
protected:
	SOCKET m_socket;
	unsigned m_type;
public:
	explicit GenericSocket(int type);
	GenericSocket(const GenericSocket& other) = default;
	virtual ~GenericSocket();

	void Bind(SocketAddress& address) const;
	void NonBlocking(bool isNonBlocking) const;

	GenericSocket& operator=(const GenericSocket& other) = default;
};