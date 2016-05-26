#pragma once 
#include <string>
#include "Transform.h"

namespace SerraPlo {
	struct GameObject {
		uint16_t materialID{ 0 };
		uint16_t modelID{ 0 };
		Transform transform;
	};
}