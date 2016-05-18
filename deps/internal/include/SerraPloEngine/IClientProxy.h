#pragma once
#include "IScreen.h"
#include "UDPStream\UDPStream.hh"

namespace SerraPlo {

	struct ClientProxy {
		sockaddr address;
		std::string nick;
		ScreenState currentScreenState;
		int screenIndex;
	};

}

