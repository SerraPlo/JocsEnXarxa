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
	std::cout << key << std::endl;
	if (strcmp(key.c_str(),"HELLO")==0){
		int assignedId=99;
		for (int i = 1; i <= MAX_PLAYERS; i++) {
			if (!id[i-1]) {
				assignedId = i;
				id[i - 1] = true;
				pAdress[i - 1] = from;
				scores[i - 1] = 0;
				std::string temp = std::string("WELCOME_"+ std::to_string(assignedId));
				std::cout << temp << std::endl;
				_sock.SendTo(temp, from);
				return true;
			}
		}		
	}
	if (strcmp(key.c_str(), "CLICK_") == 0) {
		//test if its ok
		//if yes
			scores[atoi(msg.c_str())-1] += 1;
			//killmonster
			for (int i = 1; i <= MAX_PLAYERS; i++) {
				if (id[i - 1]) _sock.SendTo("hey", pAdress[i - 1]);
		}
		return true;
	}
	if (strcmp(key.c_str(), "EXIT_") == 0) {
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
