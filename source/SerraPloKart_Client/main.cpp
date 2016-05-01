#include "AppClient.h"
#include <iostream>

int main(int argc, char** argv) {
	try {
		AppClient("127.0.0.1:5000").Run();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cin.ignore();
	}
	return 0;
}