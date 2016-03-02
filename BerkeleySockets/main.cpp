#include <SocketsLib\UDPSocket.h>
#include <iostream>
#include "ClientList.h"

const int MAX_DATA = 1300;

void server(GenericSocket* sock, SocketAddress &socketAddress) {
	sock->Bind(socketAddress);
	char data[MAX_DATA];
	ClientList clientList;
	while (1) {
		SocketAddress from;
		sock->ReceiveFrom(data, MAX_DATA, from);
		int numClient = clientList.CheckAdress(from);
		if (numClient == ClientList::NOT_FOUND)
			clientList.push_back(from), numClient = clientList.CheckAdress(from);
		if (!strcmp(data, "exit"))
			clientList.erase(clientList.begin() + numClient), printf("Client %i disconnected\n", numClient);
		else
			printf("Client %i: %s\n", numClient, data);
		if (clientList.empty()) break;
	}
}

void client(GenericSocket* sock, SocketAddress &socketAddress) {
	char data[MAX_DATA];
	while (std::cin >> data) {
		sock->SendTo(data, MAX_DATA, socketAddress);
		if (!strcmp(data, "exit")) break;
	}
}

void run(const char* appType, const char* cinAddress) {
	GenericSocket* sock = new UDPSocket;
	SocketAddress socketAddress(cinAddress);
	!strcmp(appType, "server") ? server(sock, socketAddress) : client(sock, socketAddress);
	delete sock;
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