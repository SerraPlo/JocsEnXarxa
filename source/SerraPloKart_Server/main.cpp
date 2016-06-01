#include "AppServer.h"
#include <iostream>

int main(int argc, char** argv) {
	try {
		AppServer::Instance().Run();
		std::cout << "aloba";
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cin.ignore();
	}
	return 0;
}