#include <thread>
#include "Sender.h"
#include "Receiver.h"

//const int MAX_DATA = 1300;

/*void server(UDPSocket &sock, SocketAddress &socketAddress) {
	sock.Bind(socketAddress);
	char data[MAX_DATA];
	ClientList clientList;
	while (1) {
		SocketAddress from;
		sock.ReceiveFrom(data, MAX_DATA, from);
		int numClient = clientList.CheckAdress(from);
		if (numClient == ClientList::NOT_FOUND)
			clientList.push_back(from), numClient = clientList.CheckAdress(from), printf("Client %i connected\n", numClient);
		if (!strcmp(data, "exit"))
			clientList.erase(clientList.begin() + numClient), printf("Client %i disconnected\n", numClient);
		else
			printf("Client %i: %s\n", numClient, data);
		if (clientList.empty()) break;
	}
}

void client(UDPSocket &sock, SocketAddress &socketAddress) {
	char data[MAX_DATA];
	while (std::cin.getline(data, MAX_DATA)) {
		sock.SendTo(data, MAX_DATA, socketAddress);
		if (!strcmp(data, "exit")) break;
	}
}*/

void run(const char* sendAddress, const char* recvAddress) {
	UDPSocket sock;
	Sender senderO(recvAddress);
	Receiver receiverO(sendAddress);
	std::thread rcv(receiverO);
	std::thread snd(senderO);
	rcv.join();
	snd.join();
}

int main(int argc, const char* argv[]) {
	try {
		SocketTools::BuildLibrary();
		auto cinAddressS = argv[1]; //IP:port
		auto cinAddressR = argv[2]; //IP:port
		run(cinAddressS, cinAddressR);
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