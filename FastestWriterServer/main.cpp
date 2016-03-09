#include <TCPSocket.h>
#include <iostream>
#include "ClientProxy.h"
#include "ListWords.h"

#define MAX_DATA 1300

std::vector<ClientProxy> clientList;

void SendToAll(const char* msg) {
	for (auto client : clientList) {
		client.getSocket()->Send(msg);
	}
}

bool GetKeyMsg(std::string& data, std::string toCheck) {
	std::string key = data.substr(0, data.find_last_of('_'));
	data = data.substr(key.size() + 1);
	return (key == toCheck);

}

void server(const char* bindAddress, int numPlayers) {
	SocketAddress addr;
	addr.setAddress(bindAddress);

	TCPSocket dispatcher;
	dispatcher.NonBlocking(true); //non-blocking socket enabled
	dispatcher.Bind(addr);
	dispatcher.Listen(numPlayers);

	ListWords wordsList;
	
	for (int i = 0; i < numPlayers; ++i) {	
		printf("Waiting for client %d\n", i + 1);
		SocketAddress from;
		std::shared_ptr<TCPSocket> tempSocket = dispatcher.Accept(from);
		tempSocket->NonBlocking(true);
		clientList.emplace_back(tempSocket);
		printf("Connected to client %d\n", i + 1);
	}

	SendToAll("BEGIN");

	int incompletes = numPlayers;
	char data[MAX_DATA];
	while (incompletes) {
		for (auto client : clientList) {
			if (client.getSocket()->Receive(data, MAX_DATA) > 0) {
				std::string tempData(data);
				if (GetKeyMsg(tempData, "NICK") && client.checkNick("")) {
					client.setNick(tempData);
					--incompletes;
				}
			}
		}
	}
	
}

void run(const char* bindAddress, const char* numPlayers) {

	server(bindAddress, atoi(numPlayers));
}

int main(int argc, const char* argv[]) {
	try {
		SocketTools::BuildLibrary();
		atexit(SocketTools::UnloadLibrary);//no es criden els destructors
		auto bindAddress = argv[1]; //adress_bind
		auto numPlayers = argv[2]; //num_players
		run(bindAddress, numPlayers);
	} catch (std::exception &e) {
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
