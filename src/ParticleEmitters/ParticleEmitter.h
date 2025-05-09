#pragma once

#include <optional>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <random>

#include <imgui.h>

#include <json.hpp>

#include <stb_image.h>

#include "opengl/GLUtils.h"
#include "opengl/VertexArrayObject.h"
#include "opengl/VertexBufferObject.h"

#include "Particle.h"

#include "IParticlePropertyModifier.h"
#include "VelocityModifier.h";
#include "ColourModifier.h"
#include "PositionModifier.h"
#include "LifetimeModifier.h"
#include "SizeModifier.h"

#include "IParticlePropertyOverLifetimeModifier.h"
#include "ColourOverLifetimeModifer.h"
#include "SizeOverLifetimeModifer.h"

class Serialiser;

class ParticleEmitter
{
public:

	ParticleEmitter();

	virtual ~ParticleEmitter() = default;

	virtual void LoadSetTexture();
	virtual void Destroy() = 0;

	virtual void Update(double deltaTime, glm::vec3 cameraPos) = 0;

	virtual void Render(glm::mat4 view,glm::mat4 proj) = 0;

	virtual void SpawnParticle(Particle particle, int particleCount) = 0;
	virtual void RemoveParticles(const std::vector<int>& particlesToRemove) = 0;
	virtual void GetBufferData(const Particle* particles, int particleCount, float* outArray) = 0;

	unsigned int ParticlesToEmitCount();

	void Editor();
	virtual void Reload();

	void SaveParticleSystemConfig();
	void LoadParticleSystemConfig();

	std::vector<IParticlePropertyModifier*> modifiers;
	std::vector<IParticlePropertyOverLifetimeModifer*> overLifetimeModifiers;


	//Getters for performance data
	virtual double GetUpdateTime() = 0;
	virtual double GetRenderTime() = 0;
	virtual unsigned int GetActiveParticleCount() = 0;
	virtual unsigned int GetParticleGPUSizeBytes() = 0;
	virtual unsigned int GetTotalParticlesGPUSizeBytes() = 0;
	virtual unsigned int GetTotalDataTransferBytes() = 0;
	virtual unsigned int GetTotalDrawCalls() = 0;

	std::string texturePath = "";
	unsigned int particlesPerSecond = 0;

	unsigned int particleCount = 0;
	unsigned int editorParticleCount = 0;
	unsigned int activeParticleCount = 0;

protected:

	std::string defaultTexturePath = "assets/textures/default.png";
	float timeSinceLastEmission = 0.0f;
	float emissionInterval = 0.0f;


	float squareVertices[30] = {
		// First triangle
	   -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  // Top-left     -> UV (0,1)
	   -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  // Bottom-left  -> UV (0,0)
		0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  // Bottom-right -> UV (1,0)

		// Second triangle
	   -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  // Top-left     -> UV (0,1)
		0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  // Bottom-right -> UV (1,0)
		0.5f,  0.5f,  0.0f,  1.0f, 1.0f   // Top-right    -> UV (1,1)
	};

	double updateTime;
	double renderTime;
	unsigned int frameDrawCallsCount = 0;

	GLuint uTexSlot;
	GLuint texture;
	GLuint shaderProgram;
	GLuint uMVPLoc;

	GLuint uView;
	GLuint uProjection;
};
