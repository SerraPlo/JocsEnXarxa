#include <TCPSocket.h>
#include <iostream>

#define MAX_DATA 1300

void client(const char* bindAddress, const char* nick) {
	SocketAddress addr;
	addr.setAddress(bindAddress);

	TCPSocket theSocket;
	//dispatcher.NonBlocking(true); //non-blocking socket enabled
	theSocket.Connect(addr);

	char data[MAX_DATA];
	while (true) {
		theSocket.Receive(data, MAX_DATA);
		if (strcmp(data, "BEGIN")) {
			theSocket.Send((std::string("NICK_") + nick).c_str());
			std::cout << "holi" << std::endl;
			break;
		}
	}

}

void run(const char* serverAddress, const char* nick) {

	client(serverAddress, nick);
}

int main(int argc, const char* argv[]) {
	try {
		SocketTools::BuildLibrary();
		atexit(SocketTools::UnloadLibrary);//no es criden els destructors
		auto serverAddress = argv[1]; //adress_bind
		auto nick = argv[2]; //num_players
		run(serverAddress, nick);
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