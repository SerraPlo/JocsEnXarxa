#include <TCPSocket.h>
#include <iostream>

void server(const char* bindAddress, int numPlayers) {
	TCPSocket tcpSocket;
	SocketAddress addr;
	addr.setAddress(bindAddress);
	tcpSocket.NonBlocking(true); //non-blocking socket enabled
	tcpSocket.Bind(addr);
	tcpSocket.Listen(numPlayers);

	for (int i = 0; i < numPlayers; i++) {
		ClientProxy client;
		SocketAddress from;
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
