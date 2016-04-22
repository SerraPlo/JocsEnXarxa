#pragma once
#include <vector>
#include <memory>

namespace SerraPlo {

class ParticleBatch2D;
class SpriteBatch;

class ParticleEngine2D
{
	std::vector<ParticleBatch2D*> m_batches;
public:
	explicit ParticleEngine2D() = default;
	~ParticleEngine2D();

	void addParticleBatch(ParticleBatch2D* particleBatch);
	void update(float deltaTime);
	void draw(SpriteBatch &spriteBatch);
};

}