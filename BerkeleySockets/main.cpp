#include "UDPSocket.h"
#include <iostream>

const int MAX_DATA = 1300;

void server(const char* address) {
	UDPSocket sock;
	SocketAddress addr;
	addr.setAddress(address);
	sock.Bind(addr);
	char data[MAX_DATA];
	SocketAddress a;
	while (1) {
		if (sock.ReceiveFrom(data, MAX_DATA, a)) std::cout << data << std::endl;
		if (!strcmp(data, "exit")) break;
	}
}

void client(const char* address) {
	UDPSocket sock;
	SocketAddress addr;
	addr.setAddress(address);
	char data[MAX_DATA];
	while (std::cin >> data) {
		sock.SendTo(data, MAX_DATA, addr);
		if (!strcmp(data, "exit")) break;
	}
}

void run(const char* type, const char* address) {
	
	if (!strcmp(type, "server")) server(address);
	else client(address);
}

int main(int argc, const char* argv[]) {
	try {
		SocketTools::BuildLibrary();
		const char* type = argv[1];
		const char* address = argv[2];
		run(type, address);
		SocketTools::UnloadLibrary();
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		system("pause");
	}
	return 0;
}