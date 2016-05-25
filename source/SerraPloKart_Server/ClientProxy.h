#pragma once
#define _USE_MATH_DEFINES
#include <SerraPloEngine/IScreen.h>
#include "UDPStream\UDPStream.hh"
#include <SerraPloEngine/CarPhysics.h>
using namespace SerraPlo;

struct ClientProxy {
	sockaddr address;
	std::string nick;
	ScreenState currentScreenState;
	int screenIndex;
	Transform transform;
	CarPhysics carPhy;
};