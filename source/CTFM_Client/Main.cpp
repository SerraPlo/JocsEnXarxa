#include "Game.h"
#include <SocketTools.h>

/*
Parte grafica
Envia comando -> mouse


HELLO
CLICK_x:y

*/


int main(int argc, char ** argv) {
	//Game game("Catch the fire monsters", 700, 700);

	try {
		//game.run();
		SocketTools::ThrowError("holi");
	}
	catch (std::exception e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SOCKET TOOLS  bERROR", e.what(), NULL);
	}

	return 0;
}