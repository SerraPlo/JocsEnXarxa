#pragma once
#include <SDL2\SDL.h>

namespace SerraPlo {

	class FPSLimiter {
		float m_targetFPS;
		Uint32 m_startTicks;
		Uint32 m_frameTicks;

		void calculateFPS();
	public:
		float m_fps;

		explicit FPSLimiter();
		explicit FPSLimiter(float targetFPS);
		~FPSLimiter() = default;

		void setTargetFPS(float targetFPS) { m_targetFPS = targetFPS; };
		void printFPS() const;

		void begin();
		void end();
	};

}

