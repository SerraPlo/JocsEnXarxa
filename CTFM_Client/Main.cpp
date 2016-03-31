#include "Game.h"

/*
Parte grafica
Envia comando -> mouse


HELLO
CLICK_x:y

*/


int main(int argc, char ** argv) {
	Game game("Catch the fire monsters", 700, 700);

	try {
		game.run();
	}
	catch (std::exception e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return 0;
}