#pragma once
#include "ParticleEmitter.h"

class BaseParticleEmitter : public ParticleEmitter
{
public:
	BaseParticleEmitter();
	~BaseParticleEmitter();

	void Destroy() override;

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

	std::vector<VertexArrayObject> vertexArrays;
	std::vector<VertexBufferObject> vertexBuffers;
	std::vector<VertexBufferObject> particlePropertiesBuffers;
	std::vector<Particle> particles;
	std::vector<int> renderOrderIndices;


	void GetBufferData(const Particle* particles, int particleCount, float* outArray) override;



};
