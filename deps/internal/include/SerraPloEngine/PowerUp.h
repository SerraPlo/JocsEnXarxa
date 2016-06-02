#pragma once
#include "GameObject.h"
#include <ctime>
#include <iostream>
#include <glm/glm.hpp>

namespace SerraPlo {

	struct PowerUp : GameObject {
		virtual void Update() = 0;
	};

	struct GreenShell : PowerUp {
		void Update() {
			if (enabled) {

			}
		}
	};

	class PowerUpList {
	public:
		GreenShell greenShell;
	};

}