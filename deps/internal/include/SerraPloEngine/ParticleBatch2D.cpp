#include "ParticleBatch2D.h"

namespace SerraPlo {

ParticleBatch2D::ParticleBatch2D(int maxParticles, float decayRate, const GLuint &texture, std::function<void(Particle2D&, float)> updateFunction) :
	m_maxParticles(maxParticles),
	m_decayRate(decayRate),
	m_particles(new Particle2D[m_maxParticles]),
	m_textureID(texture),
	m_updateFunction(updateFunction) {}

ParticleBatch2D::~ParticleBatch2D() {
	delete[] m_particles;
}

void ParticleBatch2D::init(int maxParticles, float decayRate, const GLuint &texture, std::function<void(Particle2D&, float)> updateFunction) {
	m_maxParticles = maxParticles;
	m_decayRate = decayRate;
	m_particles = new Particle2D[m_maxParticles];
	m_textureID = texture;
	m_updateFunction = updateFunction;
}

int ParticleBatch2D::findFreeParticle() {
	for (auto i = m_lastFreeParticle; i < m_maxParticles; i++) {
		if (m_particles[i].life <= 0.0f) {
			m_lastFreeParticle = i;
			return i;
		}
	}
	for (auto i = 0; i < m_lastFreeParticle; i++) {
		if (m_particles[i].life <= 0.0f) {
			m_lastFreeParticle = i;
			return i;
		}
	}
	return 0;
}

void ParticleBatch2D::addParticle(const glm::vec2 & position, const glm::vec2 & velocity, float width, const ColorRGBA8 & color) {
	auto particleIndex = findFreeParticle();
	auto &p = m_particles[particleIndex];
	p.position = position;
	p.velocity = velocity;
	p.life = 1.0f;
	p.width = width;
	p.color = color;
	
}

void ParticleBatch2D::pushBatch(SpriteBatch &spriteBatch) const {
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (auto i = 0; i < m_maxParticles; i++) {
		auto &p = m_particles[i];
		if (p.life > 0.0f) {
			glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
			spriteBatch.pushBatch(destRect, uvRect, m_textureID, 0.0f, p.color);
		}
	}
}

void ParticleBatch2D::update(float deltaTime) const {
	for (auto i = 0; i < m_maxParticles; i++) {
		auto &p = m_particles[i];
		if (p.life > 0.0f) {
			m_updateFunction(p, deltaTime);
			p.life -= m_decayRate*deltaTime;
		}
	}
}

}