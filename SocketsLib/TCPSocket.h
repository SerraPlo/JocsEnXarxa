#pragma once
#include "GenericSocket.h"
#include <memory>

class TCPSocket : public GenericSocket
{

public:
	explicit TCPSocket();
	explicit TCPSocket(SOCKET a);
	~TCPSocket();
	void Listen(int backlog) const;
	void Connect(SocketAddress& addr) const;
	std::shared_ptr<TCPSocket> Accept(SocketAddress& from) const;
	void Send(const void* data) const;
	int Receive(void* data, int lenData) const;
};

