#include <iostream>
#include <SocketLib/UDPSocket.h>
#include "ListOfMonsters.h"
#include <time.h>
#include <thread>

#pragma comment(lib, "SocketLib")

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
#define MAX_MONSTERS 10
#define DELAY_RAND 1000
enum class KeyMsg {
	WELCOME = 1, FIN, MONSTERS, TIME,
	HELLO, CLICK, EXIT
};

int scores[15]; //save score/player
SocketAddress pAdress[15];
bool id[15];	//know if some score is occuped
int screenWidth=700;			
int screenHeight=700;
ListOfMonsters monsters(MAX_MONSTERS);
int lastUpdate = 0;


void initArrays() {
	for (int i = 0; i < MAX_PLAYERS; i++) {
		scores[i] = 0;
		id[i] = false;
	}
	monsters.initMonsters(screenWidth, screenHeight);
	lastUpdate = clock();
}

bool ProcessMsg(const std::string &data, SocketAddress &from, UDPSocket &_sock) {
	auto pos = data.find_last_of('_');
	std::string key = data.substr(0,pos);
	std::string msg = data.substr(pos + 1, data.size() - 1);
	
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
	if (strcmp(key.c_str(), "CLICK") == 0) {
		std::cout << data << std::endl;
		auto pos = msg.find_last_of('#');
		int idFromStr = atoi(msg.substr(0, pos).c_str());
		msg = msg.substr(pos + 1, msg.size() - 1);
		
		pos = msg.find_last_of(':');
		int x = atoi(msg.substr(0, pos).c_str());
		int y = atoi(msg.substr(pos + 1, msg.size() - 1).c_str());

		if (monsters.checkIfSomeMonsterDies(x, y)) {//if coords are ok (kill monster + send to all)
			scores[idFromStr - 1]++;
			for (int i = 0; i < MAX_PLAYERS; i++) {
				if (id[i]) {
					if (monsters.getNumMonsters() > 0) {
						std::string temp = "";
						temp += std::to_string(monsters.getNumMonsters());
						for (int i = 0; i < monsters.getNumMonsters(); i++) {
							temp += "#" + std::to_string(monsters.getMonster(i).getXAtWorld()) + ":" + std::to_string(monsters.getMonster(i).getYAtWorld());
						}
						_sock.SendTo("MONSTERS_" + temp, pAdress[i]);
					}
					else {
						int counter = 0;
						for (int i = 0; i < MAX_PLAYERS; i++) if (id[i]) counter++;
						std::string temp = std::to_string(counter);
						for (int i = 0; i < MAX_PLAYERS; i++) {
							if (id[i]) {
								temp += "#Player" + std::to_string(i+1) + ":" + std::to_string(scores[i]);
							}
						}
						_sock.SendTo("SCORES_" + temp, pAdress[i]);
					}
				}
			}
		}
		return true;
	}
	if (strcmp(key.c_str(), "EXIT") == 0) {
		id[atoi(msg.c_str())] = false;
		return true;
	}
	return false;
}

void RandomizeMonsters(UDPSocket &sock) {
	bool done = false;
	while (true) {
		if (clock() > lastUpdate + DELAY_RAND && !done) {
			monsters.setRandomPosition(screenWidth, screenHeight);
			lastUpdate = clock();
			for (int i = 0; i < MAX_PLAYERS; i++) {
				if (id[i]) {
					//MONSTERS_nMonsters#x:y#x:y...
					std::string temp = "";
					temp += std::to_string(monsters.getNumMonsters());
					for (int i = 0; i < monsters.getNumMonsters(); i++) {
						temp += "#" + std::to_string(monsters.getMonster(i).getXAtWorld()) + ":" + std::to_string(monsters.getMonster(i).getYAtWorld());
					}
					sock.SendTo("MONSTERS_" + temp, pAdress[i]);
					temp = std::to_string(clock());
					sock.SendTo("TIME_" + temp.substr(0,temp.length()-3), pAdress[i]);
					if (monsters.getNumMonsters()<=0) done = true;
				}
			}
		}
	}
}

void Listen(const char* bindAddress) {
	initArrays();
	SocketAddress addr(bindAddress);
	UDPSocket socket;
	socket.Bind(addr);
	SocketAddress from;
	std::thread randomizer(RandomizeMonsters,socket);
	while (true) {
		std::string data;
		socket.ReceiveFrom(data, from);
		ProcessMsg(data, from, socket);
	}
	randomizer.join();
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
