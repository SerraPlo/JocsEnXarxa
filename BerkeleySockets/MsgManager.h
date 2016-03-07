#pragma once
#include <windows.h>
#include <vector>
#include <mutex>
#include <iostream>

class MsgManager {
	std::shared_ptr<std::vector<std::string>> m_msgPool;
	CONSOLE_SCREEN_BUFFER_INFO c;

public:
	MsgManager() {
		m_msgPool = std::make_shared<std::vector<std::string>>();
	}
	~MsgManager() = default;

	void addMsg(const std::string &msg) {
		static std::mutex m_blocker;
		m_blocker.lock();
		m_msgPool->push_back(msg);
		m_blocker.unlock();
	}
	void PrintMsg(bool sender) {
		int j = 0;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
		COORD init = c.dwCursorPosition;
		//clean
		if (sender) {
			system("cls");
		}else {
			std::string  eraseLine = "";
			for (int k = 0; k <= c.srWindow.Right; k++) eraseLine += " ";
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
			for (auto k = 0; k < c.srWindow.Bottom - 3; ++k) std::cout << eraseLine << std::endl;
		}
		//print
		for (int i = (int)min(m_msgPool->size(), c.srWindow.Bottom - 3); i > 0; --i) {
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 1, (SHORT)j });
			std::cout << m_msgPool->at(m_msgPool->size() - i);
			j++;
		}
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 1, c.srWindow.Bottom - 1 });
		std::cout << "> ";
		if(!sender) SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { (int)max(init.X,3), c.srWindow.Bottom - 1 });
		//si no sender recolocar cursor
	}


	/*void operator()() {
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
		auto prev = c.srWindow.Bottom;
		while (true) {
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
			if (prev != c.srWindow.Bottom) {
				prev = c.srWindow.Bottom;
				PrintMsg();
			}
		}
	}*/
};