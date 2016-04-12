#include "Game.h"

/*
Parte grafica
Envia comando -> mouse


HELLO
CLICK_x:y

*/


int main(int argc, char ** argv) {
	try {
		SocketTools::BuildLibrary();
		auto bindAddress = argv[1]; //adress_bind
		auto serverAddress = argv[2];
		SocketAddress Caddr(bindAddress);
		SocketAddress Saddr(serverAddress);

		Game game("Catch the fire monsters", 700, 700, Saddr, Caddr);
		game.run();

		SocketTools::UnloadLibrary();
	}
	catch (std::exception e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SOCKET TOOLS  bERROR", e.what(), NULL);
	}

	return 0;
}