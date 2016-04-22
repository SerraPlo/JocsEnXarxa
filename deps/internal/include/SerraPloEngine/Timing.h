#pragma once
#include <SDL2\SDL.h>

namespace SerraPlo {

class FPSLimiter
{
	float m_fps;
	float m_targetFPS;
	Uint32 m_startTicks;
	Uint32 m_frameTicks;
	static const int NUM_SAMPLES;

	void calculateFPS();
public:
	explicit FPSLimiter();
	explicit FPSLimiter(float targetFPS);
	~FPSLimiter() = default;

	void setTargetFPS(float targetFPS) { m_targetFPS = targetFPS; };
	void printFPS() const;
	float getFPS() const { return m_fps; };

	void begin() { m_startTicks = SDL_GetTicks(); };
	void end();
};

}

