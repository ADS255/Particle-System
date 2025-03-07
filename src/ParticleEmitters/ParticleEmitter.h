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
#include "ParticlePropertyModifier.h"
#include "EmitterProperties.h"

class ParticleEmitter
{
public:

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
	virtual void Metrics() = 0;

	void SaveParticleSystemConfig(std::string path);
	void LoadParticleSystemConfig(std::string path);

	std::vector<ParticlePropertyModifier*> modifiers;

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

	GLuint shaderProgram;
	GLuint uMVPLoc;

	GLuint uView;
	GLuint uProjection;
};
