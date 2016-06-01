#pragma once
#define _USE_MATH_DEFINES
#include "UDPStream\UDPStream.hh"
#include <SerraPloEngine/CarPhysics.h>
using namespace SerraPlo;

class ClientProxy {
public:
	sockaddr address;
	std::string nick;
	Transform transform;
	CarPhysics carPhy;
	ClientProxy() = default;
	ClientProxy(sockaddr addr, const std::string &str, int clientListSize) : address(addr), nick(str) {
		transform.position = { 180 + int(clientListSize / 2)*10, 0, 115 - (clientListSize % 2)*10 };
		transform.rotation = { 0, -90, 0 };
		carPhy.AddTransform(&transform);
	};
	~ClientProxy() = default;
};