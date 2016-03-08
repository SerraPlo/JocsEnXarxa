#pragma once
#include <windows.h>
#include <vector>
#include <mutex>
#include <iostream>
#include "Utils.h"

class MsgManager {
	std::shared_ptr<std::vector<std::string>> m_msgPool;
	CONSOLE_SCREEN_BUFFER_INFO m_screenCoords;
	std::mutex m_blocker;

public:
	MsgManager() :
		m_msgPool(std::make_shared<std::vector<std::string>>())
	{}
	~MsgManager() = default;

	void addMsg(const std::string &msg) {
		m_blocker.lock();
		m_msgPool->push_back(msg);
		m_blocker.unlock();
	}

	void PrintMsg(bool sender) {
		m_blocker.lock();
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &m_screenCoords);
		COORD init = m_screenCoords.dwCursorPosition;
		//clean
		if (sender) {
			system("cls");
		}else {
			std::string eraseLine = "";
			for (int k = 0; k <= m_screenCoords.srWindow.Right; ++k) eraseLine += ' ';
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
			for (auto k = 0; k < m_screenCoords.srWindow.Bottom - 3; ++k) std::cout << eraseLine;
		}
		//print
		int j = 0;
		for (int i = Utils::Min(m_msgPool->size(), m_screenCoords.srWindow.Bottom - 3); i > 0; --i) {
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 1, static_cast<SHORT>(j) });
			std::cout << m_msgPool->at(m_msgPool->size() - i);
			++j;
		}
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 1, m_screenCoords.srWindow.Bottom - 1 });
		std::cout << "> ";
		if(!sender) SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ static_cast<SHORT>(Utils::Max(init.X,3)), static_cast<SHORT>(m_screenCoords.srWindow.Bottom - 1) });
		//si no sender recolocar cursor
		m_blocker.unlock();
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