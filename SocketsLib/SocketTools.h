#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h>
#include <exception>
#include <string>

namespace SocketTools
{
	extern inline void ThrowError(const std::string &message) {
		auto temp{ message + "\nError code: " + std::to_string(WSAGetLastError()) };
		throw std::exception(temp.c_str());
	};
	extern inline void BuildLibrary(void) {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) ThrowError("WSAStartup could not initialise WinSock library.");
	};
	extern inline void UnloadLibrary(void) {
		if (WSACleanup() != NO_ERROR) ThrowError("WSACleanup could not clear unload WinSock library.");
	};
};

