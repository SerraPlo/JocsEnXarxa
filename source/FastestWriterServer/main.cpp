#include <iostream>
#include "Server.h"

static void run(const char* bindAddress, int numPlayers) {
	if (numPlayers <= 0) SocketTools::SL_THROW_ERROR("Number of players is less or equal to 0");
	Server server(bindAddress, numPlayers);
	server.Run();
}

int main(int argc, const char* argv[]) {
	try {
		system("cls");
		system("title Word Battle Server");
		SocketTools::BuildLibrary();
		auto bindAddress = argv[1]; //adress_bind
		auto numPlayers = argv[2]; //num_players
		run(bindAddress, atoi(numPlayers));
		SocketTools::UnloadLibrary();
	} catch (std::exception &e) {
		std::cout << "----------------" << std::endl;
		std::cout << "Exception caught" << std::endl;
		std::cout << "----------------" << std::endl;
		std::cout << e.what() << std::endl << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
		std::cin.ignore();
	}
	return 0;
}
