#pragma once

#include <optional>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <random>

#include <imgui.h>

#include <json.hpp>

#include "opengl/GLUtils.h"
#include "opengl/VertexArrayObject.h"
#include "opengl/VertexBufferObject.h"

#include "Particle.h"
#include "EmitterProperties.h"

#include "IParticlePropertyModifier.h"
#include "VelocityModifier.h";
#include "ColourModifier.h"
#include "PositionModifier.h"
#include "LifetimeModifier.h"
#include "SizeModifier.h"

class ParticleEmitter
{
public:

	ParticleEmitter();

	virtual ~ParticleEmitter() = default;

	virtual void Initialise() = 0;
	virtual void Destroy() = 0;

	virtual void Update(double deltaTime) = 0;

	virtual void Render(glm::mat4 view,glm::mat4 proj) = 0;

	virtual void SpawnParticle(Particle particle, int particleCount) = 0;
	virtual void RemoveParticles(const std::vector<int>& particlesToRemove) = 0;
	virtual void GetBufferData(const Particle* particles, int particleCount, float* outArray) = 0;

	unsigned int ParticlesToEmitCount();

	void Editor();

	void SaveParticleSystemConfig(std::string path);
	void LoadParticleSystemConfig(std::string path);

	std::vector<IParticlePropertyModifier*> modifiers;


	//Getters for performance data
	virtual double GetUpdateTime() = 0;
	virtual double GetRenderTime() = 0;
	virtual unsigned int GetActiveParticleCount() = 0;
	virtual unsigned int GetParticleGPUSizeBytes() = 0;
	virtual unsigned int GetTotalParticlesGPUSizeBytes() = 0;
	virtual unsigned int GetTotalDataTransferBytes() = 0;
	virtual unsigned int GetTotalDrawCalls() = 0;

protected:

	float RandomFloat(float min, float max);

	float squareVertices[18] = {
		// First triangle
	   -0.5f,  0.5f,  0.0f,  // Top-left
	   -0.5f, -0.5f,  0.0f,  // Bottom-left
		0.5f, -0.5f,  0.0f,  // Bottom-right

		// Second triangle
	   -0.5f,  0.5f,  0.0f,  // Top-left
		0.5f, -0.5f,  0.0f,  // Bottom-right
		0.5f,  0.5f,  0.0f   // Top-right
	};

	std::optional<EmitterProperties> properties;
	EmitterProperties editorProperties;

	double updateTime;
	double renderTime;
	unsigned int frameDrawCallsCount = 0;

	GLuint shaderProgram;
	GLuint uMVPLoc;

	GLuint uView;
	GLuint uProjection;
};
