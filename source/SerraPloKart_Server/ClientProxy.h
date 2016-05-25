#pragma once
#define _USE_MATH_DEFINES
#include <SerraPloEngine/IScreen.h>
#include "UDPStream\UDPStream.hh"
#include <SerraPloEngine/CarPhysics.h>
using namespace SerraPlo;

class ClientProxy {
public:
	sockaddr address;
	std::string nick;
	ScreenState currentScreenState{ ScreenState::NONE };
	int screenIndex{ SCREEN_INDEX_NO_SCREEN };
	Transform transform;
	CarPhysics carPhy;
	ClientProxy(sockaddr addr, const std::string &str) : address(addr), nick(str) {};
	~ClientProxy() = default;
};