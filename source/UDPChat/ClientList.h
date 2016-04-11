#pragma once
#include <vector>
#include <SocketsLib/SocketAddress.h>

class ClientList : public std::vector<SocketAddress>
{
public:
	static const int NOT_FOUND = -1;
	ClientList() = default;
	~ClientList() = default;
	int CheckAdress(SocketAddress& sa) {
		for (size_t i = 0; i < this->size(); ++i)
			if (this->at(i) == sa) return i;
		return NOT_FOUND;
	}
};