#pragma once
#include "IScreen.h"
#include "UDPStream\UDPStream.hh"

namespace SerraPlo {

	class IClientProxy {
	public:
		ScreenState currentScreenState{ ScreenState::NONE };
		int screenIndex{ SCREEN_INDEX_NO_SCREEN };
		sockaddr address;

		IClientProxy() = default;
		~IClientProxy() = default;
	};

}

