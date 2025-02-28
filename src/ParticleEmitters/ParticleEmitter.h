#pragma once

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

class ParticleEmitter
{
public:

	virtual ~ParticleEmitter() = default;

	virtual void Initialise() = 0;
	virtual void Destroy() = 0;

	virtual void Update(double deltaTime) = 0;

	virtual void Render(glm::mat4 mvp) = 0;

	virtual void SpawnParticle(Particle particle, int particleCount) = 0;
	virtual void RemoveParticle(int particleIndex) = 0;
	virtual void GetBufferData(const Particle* particles, int particleCount, float* outArray) = 0;

	unsigned int ParticlesToEmitCount();

	void Editor();

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

	float timeSinceLastEmission = 0;
	float emissionInterval = 0;
	unsigned int particlesPerSecond = 10;

	unsigned int particleCount = 100;
	unsigned int activeParticleCount = 0;

	float particleLifetime = 10;
	float colour[4] = { 1.0,1.0,1.0,1.0 };
	float size = 1.0f;
	glm::vec3 position = glm::vec3();
	glm::vec3 velocity = glm::vec3();

	bool randomiseLifetime = false;
	bool randomiseSize = false;
	bool randomisePosition = false;
	bool randomiseVelocity = false;
	bool randomiseColour = false;

	float minLifetime = 0.0f;
	float maxLifetime = 0.0f;
	float minSize, maxSize;
	glm::vec3 minPosition = glm::vec3();
	glm::vec3 maxPosition = glm::vec3();
	glm::vec3 minVelocity = glm::vec3();
	glm::vec3 maxVelocity = glm::vec3();

	GLuint shaderProgram;
	GLuint uMVPLoc;
};
