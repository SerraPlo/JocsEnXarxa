#pragma once
#include <windows.h>
#include <vector>
#include <mutex>
#include <iostream>

class MsgManager {
	std::vector<std::string> m_msgPool;
	CONSOLE_SCREEN_BUFFER_INFO c;

public:
	MsgManager() = default;
	~MsgManager() = default;

	void addMsg(const std::string &msg) {
		static std::mutex m_blocker;
		m_blocker.lock();
		m_msgPool.push_back(msg);
		m_blocker.unlock();
	}
	void PrintMsg() {
		system("cls");
		int j = 0;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
		for (auto i = min(m_msgPool.size(), c.srWindow.Bottom - 3); i > (unsigned) 0; --i) {
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 1, (SHORT)j });
			std::cout << m_msgPool.at(m_msgPool.size() - i);
			j++;
		}
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 1, c.srWindow.Bottom - 1 });
		std::cout << "> ";
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