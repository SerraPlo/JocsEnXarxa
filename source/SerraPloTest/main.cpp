#include "App.h"
#include <iostream>

int main(int argc, char** argv) {
	try {
		App app;
		app.run();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}