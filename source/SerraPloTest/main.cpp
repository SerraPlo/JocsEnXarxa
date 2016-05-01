#include "App.h"
#include <iostream>

int main(int argc, char** argv) {
	try {
		AppClient app;
		app.Run();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cin.ignore();
	}
	return 0;
}