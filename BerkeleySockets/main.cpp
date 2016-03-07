#include <thread>
#include "Sender.h"
#include "Receiver.h"

/*VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr) {
	printf("Resize event\n");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
}
VOID KeyEventProc(KEY_EVENT_RECORD ker)
{
	printf("Key event: ");

	if (ker.bKeyDown)
		printf("key pressed\n");
	else printf("key released\n");
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	printf("Mouse event: ");

	switch (mer.dwEventFlags)
	{
	case 0:

		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			printf("left button press \n");
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			printf("right button press \n");
		}
		else
		{
			printf("button press\n");
		}
		break;
	case DOUBLE_CLICK:
		printf("double click\n");
		break;
	case MOUSE_HWHEELED:
		printf("horizontal mouse wheel\n");
		break;
	case MOUSE_MOVED:
		printf("mouse moved\n");
		std::cout << mer.dwMousePosition.X;
		break;
	case MOUSE_WHEELED:
		printf("vertical mouse wheel\n");
		break;
	default:
		printf("unknown\n");
		break;
	}
}*/
void run(const char* sendAddress, const char* recvAddress) {
	/*HANDLE hStdin;
	DWORD fdwSaveOldMode;

	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];
	int counter = 0;

	// Get the standard input handle. 
	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	// Save the current input mode, to be restored on exit. 
	GetConsoleMode(hStdin, &fdwSaveOldMode);

	// Enable the window and mouse input events. 

	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	SetConsoleMode(hStdin, fdwMode);

	// Loop to read and handle the next 100 input events. 

	while (true)
	{
		// Wait for the events. 

		ReadConsoleInput(hStdin,irInBuf,128,&cNumRead);

		// Dispatch the events to the appropriate handler. 

		for (i = 0; i < cNumRead; i++)	{

			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input 
				printf("2");//KeyEventProc(irInBuf[i].Event.KeyEvent);
				break;

			case MOUSE_EVENT: // mouse input 
				printf("3");
				MouseEventProc(irInBuf[i].Event.MouseEvent);
				break;

			case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
				printf("4");
				ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
				break;

			default:
				//printf("Unknown event type\n");
				break;
			}
		}
	}

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);
	*/
	
	//--------------------------------
	std::shared_ptr<MsgManager> thePool = std::make_shared<MsgManager>();
	Sender senderO(recvAddress, thePool.get());
	Receiver receiverO(sendAddress, thePool.get());
	std::thread snd(senderO);
	std::thread rcv(receiverO);
	snd.join();
	rcv.join();
}

int main(int argc, const char* argv[]) {
	try {
		SocketTools::BuildLibrary();
		auto sendAddress = argv[1]; //IP:port
		auto recvAddress = argv[2]; //IP:port
		std::cout << sendAddress << std::endl;
		std::cout << recvAddress << std::endl;
		run(sendAddress, recvAddress);
		SocketTools::UnloadLibrary();
	} 
	catch (std::exception &e) {
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