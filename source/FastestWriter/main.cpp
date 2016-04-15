#include <iostream>
#include "Client.h"

static void run(const char* serverAddress, const char* nick) {
	Client mainClient(serverAddress, nick);
	mainClient.Run();
}

int main(int argc, const char* argv[]) {
	try {
		system("cls");
		system("title Word Battle Client");
		SocketTools::BuildLibrary();
		atexit(SocketTools::UnloadLibrary); // CAUTION: objects destructors are not called by default
		auto serverAddress = argv[1]; // adress_server
		auto nick = argv[2]; // nick
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