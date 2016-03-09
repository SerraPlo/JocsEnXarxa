#include <thread>
#include "Sender.h"
#include "Receiver.h"

void run(const char* sendAddress, const char* recvAddress) {
	MsgManager thePool;
	Sender senderO(recvAddress, thePool);
	Receiver receiverO(sendAddress, thePool);
	std::thread snd(senderO);
	std::thread rcv(receiverO);
	snd.join();
	rcv.join();
}

int main(int argc, const char* argv[]) {
	try {
		SocketTools::BuildLibrary();
		atexit(SocketTools::UnloadLibrary);//no es crida ~ del socket
		auto sendAddress = argv[1]; //IP:port
		auto recvAddress = argv[2]; //IP:port
		run(sendAddress, recvAddress);
	} catch (std::exception &e) {
		std::cout << "----------------" << std::endl;
		std::cout << "Exception caught" << std::endl;
		std::cout << "----------------" << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
		std::cin.ignore();
	}
	return 0;
}
