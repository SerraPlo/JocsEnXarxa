#pragma once
#include "GenericSocket.h"
#include <memory>

class TCPSocket : public GenericSocket
{

public:
	explicit TCPSocket();
	explicit TCPSocket(SOCKET a);
	~TCPSocket();
	void Listen(int backlog);
	void Connect(SocketAddress& addr);
	std::shared_ptr<TCPSocket> Accept(SocketAddress& from);
	void Send(const void* data);
	void Receive(void* data, int lenData);
};

