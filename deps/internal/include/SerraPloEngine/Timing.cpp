#include "Timing.h"
#include <iostream>

namespace SerraPlo {

const int FPSLimiter::NUM_SAMPLES = 10;

FPSLimiter::FPSLimiter() :
	m_fps(0), 
	m_targetFPS(120.0f), 
	m_startTicks(0),
	m_frameTicks(0) {}

FPSLimiter::FPSLimiter(float targetFPS) : 
	m_fps(0),
	m_targetFPS(targetFPS),
	m_startTicks(0),
	m_frameTicks(0) {}

void FPSLimiter::calculateFPS() {
	static float frameTimes[NUM_SAMPLES];
	static auto curFrame = 0;

	static auto prevTicks = SDL_GetTicks();
	auto curTicks = SDL_GetTicks();

	frameTimes[curFrame] = static_cast<float>(curTicks - prevTicks);
	prevTicks = curTicks;

	static auto count = 0;
	curFrame++;
	if (count < NUM_SAMPLES) count = curFrame;
	else count = NUM_SAMPLES;
	if (curFrame == NUM_SAMPLES) curFrame = 0/*, std::cout << _fps << std::endl*/;

	float frameTimeAverage = 0;
	for (auto i = 0; i < count; i++) frameTimeAverage += frameTimes[i];
	frameTimeAverage /= count;

	if (frameTimeAverage > 0) m_fps = 1000.0f / frameTimeAverage;
	else m_fps = 60.0f;
}

void FPSLimiter::printFPS() const {
	static auto fpsTimer = 0;
	if (fpsTimer == 10) fpsTimer = 0, std::cout << m_fps << std::endl;
	fpsTimer++;
}

void FPSLimiter::end() {
	calculateFPS();
	m_frameTicks = SDL_GetTicks() - m_startTicks;
	if (1000.0f / m_targetFPS > m_frameTicks) SDL_Delay(static_cast<Uint32>(1000.0f / m_targetFPS - m_frameTicks));
}

}