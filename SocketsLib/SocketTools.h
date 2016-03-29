#pragma once
#include <exception>
#include <string>

#ifdef _WIN32 // Windows x64 and x86
	#define WIN32_LEAN_AND_MEAN
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0501  // Windows XP
	#endif
	#include <Ws2tcpip.h>
	#include <winsock2.h>
#else // Linux and Mac
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <netdb.h>  // getaddrinfo() and freeaddrinfo()
	#include <unistd.h> // close()
#endif

namespace SocketTools
{
#define ThrowError(message) ShowError(message, __FILE__, __LINE__);
	inline void ShowError(std::string &&message, const std::string& fileName, int line) {
#ifdef _WIN32
		message.append("\nFile: " + fileName + "\nLine: " + std::to_string(line) + "\nWin32 error code: " + std::to_string(WSAGetLastError()));
#endif
		throw std::exception(message.c_str());
	};
	inline void BuildLibrary(void) {
#ifdef _WIN32
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) ThrowError("WSAStartup could not initialise WinSock library.");
#endif
	};
	inline void UnloadLibrary(void) {
#ifdef _WIN32
		if (WSACleanup() != NO_ERROR) ThrowError("WSACleanup could not clear unload WinSock library.");
#endif
	};
};

