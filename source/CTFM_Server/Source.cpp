#include <iostream>
#include <UDPSocket.h>
/*
Genera posiciones monstruo
Valida s le damos a monstruo
Control de tiempo
Control # monstruos -> fins de partida
Control score x player


WELCOME_id
FIN_puntuacio
MONSTERS_nMonsters#x:y#x:y...
TIME_h

HELLO
CLICK_id#x,y
EXIT_id

*/

#define MAX_PLAYERS 15
enum class KeyMsg {
	WELCOME = 1, FIN, MONSTERS, TIME,
	HELLO, CLICK, EXIT
};

int scores[15]; //save score/player
SocketAddress pAdress[15];
bool id[15];	//know if some score is occuped

void initArrays() {
	for (int i = 0; i < MAX_PLAYERS; i++) {
		scores[i] = 0;
		id[i] = false;
	}
}

bool ProcessMsg(const std::string &data, SocketAddress &from, UDPSocket &_sock) {
	auto pos = data.find_last_of('_');
	std::string key = data.substr(0,pos);
	std::string msg = data.substr(pos + 1, data.size() - 1);

	SocketAddress temp("127.0.0.1:32000");

	if (strcmp(key.c_str(),"HELLO")==0){
		int assignedId;
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (!id[i]) {
				assignedId = i;
				id[i] = true;
				pAdress[i] = from;
				scores[i] = 0;
				break;
			}
		}
		std::cout << assignedId << std::endl;
		_sock.SendTo("asdfsdfafasdfa", from);
		return true;
	}
	if (strcmp(key.c_str(), "CLICK") == 0) {
		//test if its ok
		//if yes
		scores[atoi(msg.c_str())] += 1;
		//killmonster
		std::cout << from << std::endl;
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (id[i]) _sock.SendTo("exit", MAX_BYTES, pAdress[i]);
		}
		return true;
	}
	if (strcmp(key.c_str(), "EXIT") == 0) {
		std::cout << atoi(msg.c_str()) << std::endl;
		id[atoi(msg.c_str())] = false;
		return true;
	}
	return false;
}

void Listen(const char* bindAddress) {
	initArrays();
	SocketAddress addr(bindAddress);
	UDPSocket socket;
	socket.Bind(addr);
	SocketAddress from;
	while (true) {
		std::string data;
		socket.ReceiveFrom(data, from);
		std::cout << from << std::endl;
		ProcessMsg(data, from, socket);

	}
}

int main(int argc, const char* argv[]) {
	try {
		system("cls");
		system("title CTFM");
		SocketTools::BuildLibrary();
		auto bindAddress = argv[1]; //adress_bind
		Listen(bindAddress);
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
