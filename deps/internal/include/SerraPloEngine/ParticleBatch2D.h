#pragma once
#include <functional>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"

namespace SerraPlo {

class Particle2D
{
public:
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 velocity = glm::vec2(0.0f);
	float life = 0.0f;
	float width = 0.0f;
	ColorRGBA8 color;
};

inline void defaultParticleUpdate(Particle2D& particle, float deltaTime) {
	particle.position += particle.velocity*deltaTime;
}

class ParticleBatch2D
{
	int m_maxParticles = 0;
	int m_lastFreeParticle = 0;
	float m_decayRate = 0.1f;
	Particle2D* m_particles = nullptr;
	GLuint m_textureID;
	std::function<void(Particle2D&, float)> m_updateFunction;

	int findFreeParticle();
public:
	explicit ParticleBatch2D() = default;
	explicit ParticleBatch2D(int maxParticles, float decayRate, const GLuint &texture, std::function<void(Particle2D&, float)> updateFunction = defaultParticleUpdate);
	~ParticleBatch2D();

	void init(int maxParticles, float decayRate, const GLuint &texture, std::function<void(Particle2D&, float)> updateFunction = defaultParticleUpdate);
	void addParticle(const glm::vec2 &position, const glm::vec2 &velocity, float width, const ColorRGBA8 &color = ColorRGBA8{55, 255, 255, 255});

	void pushBatch(SpriteBatch &spriteBatch) const;
	void update(float deltaTime) const;
};

}
