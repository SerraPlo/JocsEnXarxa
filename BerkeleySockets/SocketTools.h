#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h>
#include <exception>
#include <string>

class SocketTools
{
public:
	static void BuildLibrary() {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ShowError("WSAStartup could not initialise WinSock library");
	};
	static void UnloadLibrary() {
		if (WSACleanup() != 0) ShowError("WSACleanup could not clear unload WinSock library");
	};
	static void ShowError(const std::string &message) {
		throw std::exception(message.c_str() + '\n' + WSAGetLastError());
	};
};

