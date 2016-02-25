#include "UDPSocket.h"
#include <iostream>

const int MAX_DATA = 1300;

void server(const char* cinAddress) {
	GenericSocket* sock = new UDPSocket;
	SocketAddress socketAddress;
	socketAddress.setAddress(cinAddress);
	sock->Bind(socketAddress);
	char data[MAX_DATA];
	SocketAddress from;
	while (1) {
		if (sock->ReceiveFrom(data, MAX_DATA, from)) std::cout << data << std::endl;
		if (!strcmp(data, "exit")) break;
	}
}

void client(const char* cinAddress) {
	GenericSocket* sock = new UDPSocket;
	SocketAddress socketAddress;
	socketAddress.setAddress(cinAddress);
	char data[MAX_DATA];
	while (std::cin >> data) {
		sock->SendTo(data, MAX_DATA, socketAddress);
		if (!strcmp(data, "exit")) break;
	}
}

void run(const char* appType, const char* cinAddress) {
	(!strcmp(appType, "server")) ? server(cinAddress) : client(cinAddress);
}

int main(int argc, const char* argv[]) {
	try {
		SocketTools::BuildLibrary();
		auto appType = argv[1]; //client/server
		auto cinAddress = argv[2]; //IP:port
		run(appType, cinAddress);
		SocketTools::UnloadLibrary();
	} 
	catch (std::exception &e) {
		std::cout << "----------------" << std::endl;
		std::cout << "Exception caught" << std::endl;
		std::cout << "----------------" << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
		std::cin.ignore();
	}
	return 0;
}