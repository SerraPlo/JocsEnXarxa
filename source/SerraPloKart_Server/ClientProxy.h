#pragma once
#include <SerraPloEngine/IScreen.h>
#include "UDPStream\UDPStream.hh"
using namespace SerraPlo;

struct ClientProxy {
	sockaddr address;
	std::string nick;
	ScreenState currentScreenState;
	int screenIndex;
};