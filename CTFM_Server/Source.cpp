#include <iostream>
#include <UDPSocket.h>

/*
Genera posiciones monstruo
Valida s le damos a monstruo
Control de tiempo
Control # monstruos -> fins de partida
Control score x player


WELCOME
BEGIN
FIN_puntuacio
MONSTERS_nMonsters#x:y#x:y...
TIME_h
*/


enum class KeyMsg {
	WELCOME = 1, BEGIN, FIN, MONSTERS, TIME
};
bool ProcessMsg(int id, const std::string &data) {
	auto pos = data.find_last_of('_');
	KeyMsg key = KeyMsg(atoi(data.substr(0, pos).c_str()));
	std::string msg = data.substr(pos + 1, data.size() - 1);

	switch (key) {
		case KeyMsg::WELCOME: {

		} return true;
		case KeyMsg::BEGIN: {

		} return true;
		case KeyMsg::FIN: {

		} return true;
		case KeyMsg::MONSTERS: {

		} return true;
		case KeyMsg::TIME: {

		} return true;
	}
}

void Listen() {
	while (true) {

	}
}

int main(int argc, const char* argv[]) {
	try {
		system("cls");
		system("title CTFM");
		SocketTools::BuildLibrary();
		auto bindAddress = argv[1]; //adress_bind
		Listen();
		SocketTools::UnloadLibrary();
	}
	catch (std::exception &e) {
		std::cout << "----------------" << std::endl;
		std::cout << "Exception caught" << std::endl;
		std::cout << "----------------" << std::endl;
		std::cout << e.what() << std::endl << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
		std::cin.ignore();
	}
	return 0;
}
