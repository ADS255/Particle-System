#include "BaseParticleEmitter_1.h"

BaseParticleEmitter_1::BaseParticleEmitter_1() {
	vertexArrayObject = VertexArrayObject();
	vertexArrayObject.Bind();

	billboardVBO = VertexBufferObject(this->squareVertices, sizeof(this->squareVertices));
	vertexArrayObject.LinkAttribute(billboardVBO, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	vertexArrayObject.LinkAttribute(billboardVBO, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


	particleDataVBO = VertexBufferObject(NULL, sizeof(float) * 8);

	GLsizeiptr stride = sizeof(float) * 8;

	// Link the position attribute (layout 1)
	vertexArrayObject.LinkAttribute(particleDataVBO,
		2,                  // Attribute layout index
		3,                  // Number of components (x, y, z)
		GL_FLOAT,           // Data type
		GL_FALSE,           // Not normalized
		stride,             // Total size of one vertex
		(void*)0);          // Offset to position data

	glVertexAttribDivisor(2, 1);

	// Link the color attribute (layout 2) colour
	vertexArrayObject.LinkAttribute(particleDataVBO,
		3,
		4,
		GL_FLOAT,
		GL_FALSE,
		stride,
		(void*)(sizeof(float) * 3));

	glVertexAttribDivisor(3, 1);

	// Link the size attribute (layout 3) size
	vertexArrayObject.LinkAttribute(particleDataVBO,
		4,
		1,
		GL_FLOAT,
		GL_FALSE,
		stride,
		(void*)(sizeof(float) * 7));

	glVertexAttribDivisor(4, 1);

	unsigned int vertexShader = GLUtils::LoadShader("shaders/baseParticleEmitter.vert", GL_VERTEX_SHADER);
	unsigned int fragmentShader = GLUtils::LoadShader("shaders/baseParticleEmitter.frag", GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "Shader program linking error:\n" << infoLog << std::endl;
	}

	uView = glGetUniformLocation(shaderProgram, "uView");
	uProjection = glGetUniformLocation(shaderProgram, "uProj");

	uTexSlot = glGetUniformLocation(shaderProgram, "uTexSlot");

}

BaseParticleEmitter_1::~BaseParticleEmitter_1()
{
	vertexArrayObject.Delete();
	billboardVBO.Delete();
	Destroy();
}

void BaseParticleEmitter_1::Destroy()
{
	particleDataVBO.Delete();
	particles.clear();
}

void BaseParticleEmitter_1::Reload()
{
	ParticleEmitter::Reload();

	particleDataVBO = VertexBufferObject(NULL, sizeof(float) * 8 * particleCount);

	GLsizeiptr stride = sizeof(float) * 8;

	// Link the position attribute (layout 1)
	vertexArrayObject.LinkAttribute(particleDataVBO,
		2,                  // Attribute layout index
		3,                  // Number of components (x, y, z)
		GL_FLOAT,           // Data type
		GL_FALSE,           // Not normalized
		stride,             // Total size of one vertex
		(void*)0);          // Offset to position data

	glVertexAttribDivisor(2, 1);

	// Link the color attribute (layout 2) colour
	vertexArrayObject.LinkAttribute(particleDataVBO,
		3,
		4,
		GL_FLOAT,
		GL_FALSE,
		stride,
		(void*)(sizeof(float) * 3));

	glVertexAttribDivisor(3, 1);

	// Link the size attribute (layout 3) size
	vertexArrayObject.LinkAttribute(particleDataVBO,
		4,
		1,
		GL_FLOAT,
		GL_FALSE,
		stride,
		(void*)(sizeof(float) * 7));

	glVertexAttribDivisor(4, 1);
}

void BaseParticleEmitter_1::Update(double deltaTime, glm::vec3 cameraPos)
{
	if (particleCount <= 0)
	{
		return;
	}

	using Clock = std::chrono::high_resolution_clock;
	auto frame_start = Clock::now();

	timeSinceLastEmission += deltaTime;
	if (timeSinceLastEmission >= emissionInterval && activeParticleCount < particleCount) {

		int particlesToEmit = ParticlesToEmitCount();

		timeSinceLastEmission = 0;

		for (size_t i = 0; i < particlesToEmit; i++)
		{
			Particle particle = Particle();

			for (size_t j = 0; j < modifiers.size(); j++)
			{
				modifiers[j]->ApplyModifier(particle, deltaTime);
			}

			SpawnParticle(particle, 1);
			activeParticleCount++;
		}
	}

	std::vector<int> particlesToDelete = std::vector<int>();

	for (size_t i = 0; i < particles.size(); ++i) {

		particles[i].lifetime -= deltaTime;

		if (particles[i].lifetime <= 0)
		{
			particlesToDelete.push_back(i);
			continue;
		}

		particles[i].position.x += particles[i].velocity.x * deltaTime;
		particles[i].position.y += particles[i].velocity.y * deltaTime;
		particles[i].position.z += particles[i].velocity.z * deltaTime;

		for (size_t j = 0; j < overLifetimeModifiers.size(); j++)
		{
			IParticlePropertyOverLifetimeModifer* modifier = overLifetimeModifiers[j];

			if (!modifier->enabled)
			{
				continue;
			}

			modifier->ApplyModifier(particles[i], deltaTime);
		}
	}

	RemoveParticles(particlesToDelete);

	if (activeParticleCount > 0)
	{
		float* particleData = new float[activeParticleCount * 8];

		GetBufferData(&particles[0], activeParticleCount, particleData);

		particleDataVBO.Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * activeParticleCount * 8, particleData);
		delete[] particleData;
	}



	auto frame_end = Clock::now();
	updateTime = std::chrono::duration<double, std::milli>(frame_end - frame_start).count();
}

void BaseParticleEmitter_1::Render(glm::mat4 view, glm::mat4 proj)
{
	using Clock = std::chrono::high_resolution_clock;
	auto frame_start = Clock::now();
	//GLuint timeQueryID;
	//glGenQueries(1, &timeQueryID);       // Create query for time elapsed


	//glBeginQuery(GL_TIME_ELAPSED, timeQueryID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);  // Disable writing to the depth buffer
	glEnable(GL_DEPTH_TEST); // Keep depth testing enabled


	vertexArrayObject.Bind();
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(proj));
	glUniform1i(uTexSlot, 0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, activeParticleCount);

	glDepthMask(GL_TRUE);

	// End the queries

	//glEndQuery(GL_TIME_ELAPSED);

	// Retrieve query results
	//GLuint64 elapsedTime = 0;
	//glGetQueryObjectui64v(timeQueryID, GL_QUERY_RESULT, &elapsedTime);

	// Print the results
	//std::cout << "GPU Time for Draw Calls: " << elapsedTime / 1e6 << " ms" << std::endl;

	// Cleanup
	//glDeleteQueries(1, &timeQueryID);

	auto frame_end = Clock::now();
	renderTime = std::chrono::duration<double, std::milli>(frame_end - frame_start).count();
}

void BaseParticleEmitter_1::SpawnParticle(Particle particle, int particleCount)
{
	particles.push_back(Particle(particle));
}

void BaseParticleEmitter_1::RemoveParticles(const std::vector<int>& particlesToRemove)
{
	std::vector<int> sortedIndices = particlesToRemove;
	std::sort(sortedIndices.rbegin(), sortedIndices.rend());

	for (int index : sortedIndices) {
		particles.erase(particles.begin() + index);
		activeParticleCount--;
	}
}


void BaseParticleEmitter_1::GetBufferData(const Particle* particles, int particleCount, float* outArray)
{
	int index = 0;

	for (int i = 0; i < particleCount; ++i)
	{
		const Particle& p = particles[i];

		outArray[index++] = p.position.x;
		outArray[index++] = p.position.y;
		outArray[index++] = p.position.z;

		outArray[index++] = p.colour[0];
		outArray[index++] = p.colour[1];
		outArray[index++] = p.colour[2];
		outArray[index++] = p.colour[3];

		outArray[index++] = p.size;
	}
}

double BaseParticleEmitter_1::GetUpdateTime()
{
	return updateTime;
}

double BaseParticleEmitter_1::GetRenderTime()
{
	return renderTime;
}

unsigned int BaseParticleEmitter_1::GetActiveParticleCount()
{
	return activeParticleCount;

}

unsigned int BaseParticleEmitter_1::GetParticleGPUSizeBytes()
{
	return sizeof(float) * 8;
}

unsigned int BaseParticleEmitter_1::GetTotalParticlesGPUSizeBytes()
{
	return activeParticleCount * GetParticleGPUSizeBytes();
}

unsigned int BaseParticleEmitter_1::GetTotalDataTransferBytes()
{
	return activeParticleCount * GetParticleGPUSizeBytes();
}

unsigned int BaseParticleEmitter_1::GetTotalDrawCalls()
{
	return 1;
}


