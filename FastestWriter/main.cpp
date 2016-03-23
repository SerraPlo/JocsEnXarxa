#include <TCPSocket.h>
#include <iostream>
#include "Client.h"
#include "UserInput.h"

static void run(const char* serverAddress, const char* nick) {
	UserData userData;
	UserInput userInput(&userData);
	std::thread inThread(userInput);
	Client client(serverAddress, nick, userData);
	client.Run();
}

int main(int argc, const char* argv[]) {
	try {
		SocketTools::BuildLibrary();
		atexit(SocketTools::UnloadLibrary);//no es criden els destructors
		auto serverAddress = argv[1]; //adress_server
		auto nick = argv[2]; //nick
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