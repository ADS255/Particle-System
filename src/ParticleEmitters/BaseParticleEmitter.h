#pragma once
#include "ParticleEmitter.h"

class BaseParticleEmitter : public ParticleEmitter
{
public:
	BaseParticleEmitter(unsigned int particleCount, float particleLifetime);
	~BaseParticleEmitter();

	void Update(double deltaTime) override;

	void Render(glm::mat4 mvp) override;

private:

	std::vector<VertexArrayObject> vertexArrays;
	std::vector<VertexBufferObject> vertexBuffers;
	std::vector<VertexBufferObject> particlePropertiesBuffers;
	std::vector<Particle> particles;
};
