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
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) ThrowError("WSAStartup could not initialise WinSock library.");
	};
	static void UnloadLibrary() {
		if (WSACleanup() != NO_ERROR) ThrowError("WSACleanup could not clear unload WinSock library.");
	};
	static void ThrowError(const std::string &message) {
		auto temp{ message + "\nError code: " + std::to_string(WSAGetLastError()) };
		throw std::exception(temp.c_str());
	};
};

