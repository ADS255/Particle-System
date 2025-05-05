#pragma once
#include "ParticleEmitter.h"

class BaseParticleEmitter_1 : public ParticleEmitter
{
public:
	BaseParticleEmitter_1();
	~BaseParticleEmitter_1();

	void Destroy() override;

	void Reload() override;

	void Update (double deltaTime,glm::vec3 cameraPos) override;

	void Render(glm::mat4 view, glm::mat4 proj) override;

	void SpawnParticle(Particle particle, int particleCount) override;

	void RemoveParticles(const std::vector<int>& particlesToRemove) override;


	double GetUpdateTime() override;

	double GetRenderTime() override;

	unsigned int GetActiveParticleCount() override;

	unsigned int GetParticleGPUSizeBytes() override;

	unsigned int GetTotalParticlesGPUSizeBytes() override;

	unsigned int GetTotalDataTransferBytes() override;


	unsigned int GetTotalDrawCalls() override;

private:
	std::vector<Particle> particles;

	VertexArrayObject vertexArrayObject;
	VertexBufferObject billboardVBO;
	VertexBufferObject particleDataVBO;



	void GetBufferData(const Particle* particles, int particleCount, float* outArray) override;
};
