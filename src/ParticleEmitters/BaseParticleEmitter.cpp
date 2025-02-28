#include "BaseParticleEmitter.h"

BaseParticleEmitter::BaseParticleEmitter(Particle particle, unsigned int particleCount)
{
	modifiers = std::vector<ParticlePropertyModifier*>();
}

BaseParticleEmitter::~BaseParticleEmitter()
{
	Destroy();
}

void BaseParticleEmitter::Initialise()
{
	vertexArrays = std::vector<VertexArrayObject>();
	vertexBuffers = std::vector<VertexBufferObject>();
	particlePropertiesBuffers = std::vector<VertexBufferObject>();

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

	uMVPLoc = glGetUniformLocation(shaderProgram, "uMVP");
}

void BaseParticleEmitter::Destroy()
{
	for (size_t i = 0; i < particles.size(); i++)
	{
		vertexArrays[i].Delete();
		vertexBuffers[i].Delete();
		particlePropertiesBuffers[i].Delete();
	}

	particles.clear();
	vertexArrays.clear();
	vertexArrays.clear();
	vertexBuffers.clear();
	particlePropertiesBuffers.clear();
}

void BaseParticleEmitter::Update(double deltaTime)
{
	if (!properties)
	{
		return;
	}
	properties->timeSinceLastEmission += deltaTime;
	if (properties->timeSinceLastEmission >= properties->emissionInterval && properties->activeParticleCount < properties->particleCount) {

		int particlesToEmit = ParticlesToEmitCount();

		properties->timeSinceLastEmission = 0;


		for (size_t i = 0; i < particlesToEmit; i++)
		{
			Particle particle(properties->colour, properties->position, properties->velocity, properties->size, properties->particleLifetime);

			if (properties->randomisePosition)
			{
				particle.position.x = RandomFloat(properties->minPosition.x, properties->maxPosition.x);
				particle.position.y = RandomFloat(properties->minPosition.y, properties->maxPosition.y);
				particle.position.z = RandomFloat(properties->minPosition.z, properties->maxPosition.z);
			}

			if (properties->randomiseVelocity)
			{
				particle.velocity.x = RandomFloat(properties->minVelocity.x, properties->maxVelocity.x);
				particle.velocity.y = RandomFloat(properties->minVelocity.y, properties->maxVelocity.y);
				particle.velocity.z = RandomFloat(properties->minVelocity.z, properties->maxVelocity.z);
			}

			if (properties->randomiseSize)
			{
				particle.size = RandomFloat(properties->minSize, properties->maxSize);
			}

			if (properties->randomiseLifetime)
			{
				particle.lifetime = RandomFloat(properties->minLifetime, properties->maxLifetime);
			}

			if (properties->randomiseColour)
			{
				particle.colour[0] = RandomFloat(0.0f, 1.0f);
				particle.colour[1] = RandomFloat(0.0f, 1.0f);
				particle.colour[2] = RandomFloat(0.0f, 1.0f);
				particle.colour[3] = RandomFloat(0.0f, 1.0f);
			}

			SpawnParticle(particle, 1);
			properties->activeParticleCount++;
		}

	}


	std::vector<int> particlesToDelete = std::vector<int>();
	float* particleData = new float[8];

	for (size_t i = 0; i < particles.size(); ++i) {
		VertexBufferObject particlePropertiesBuffer = particlePropertiesBuffers[i];

		particles[i].lifetime -= deltaTime;

		if (particles[i].lifetime <= 0)
		{
			particlesToDelete.push_back(i);
			continue;
		}

		particles[i].position.x += particles[i].velocity.x * deltaTime;
		particles[i].position.y += particles[i].velocity.y * deltaTime;
		particles[i].position.z += particles[i].velocity.z * deltaTime;


		glm::vec3 sphereCenter = glm::vec3(0.0f, 0.0f, 0.0f); // Center of sphere
		float sphereRadius = 30.0f; // Radius of sphere

		// Compute vector from center to particle
		glm::vec3 vecToParticle = particles[i].position - sphereCenter;

		// Compute squared distance (avoiding expensive sqrt operation unless necessary)
		float distSq = glm::dot(vecToParticle, vecToParticle);
		float radiusSq = sphereRadius * sphereRadius;

		// If outside the sphere, reflect the velocity
		if (distSq > radiusSq) {
			float dist = sqrt(distSq); // Compute actual distance
			glm::vec3 normal = vecToParticle / dist; // Normalize to get collision normal

			// Reflect velocity using the reflection formula
			particles[i].velocity -= 2.0f * glm::dot(particles[i].velocity, normal) * normal;

			// Move particle back to the surface to prevent sticking
			//particles[i].position = sphereCenter + normal * sphereRadius;
		}

		float startColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
		float endColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

		float factor = particles[i].lifetime / 10;
		if (factor < 0.0f) factor = 0.0f;
		if (factor > 1.0f) factor = 1.0f;

		// Interpolate color
		for (int j = 0; j < 4; j++) {
			particles[i].colour[j] = startColor[j] + (endColor[j] - startColor[j]) * (1.0f - factor);
		}

		for (size_t j = 0; j < modifiers.size(); j++)
		{
			modifiers[j]->ApplyModifier(particles[i], deltaTime);
		}

		GetBufferData(&particles[i], 1, particleData);
		glBindBuffer(GL_ARRAY_BUFFER, particlePropertiesBuffer.ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 8, particleData);
	}

	RemoveParticles(particlesToDelete);
	delete[] particleData;
}

void BaseParticleEmitter::Render(glm::mat4 mvp)
{
	//GLuint timeQueryID;
	//glGenQueries(1, &timeQueryID);       // Create query for time elapsed


	//glBeginQuery(GL_TIME_ELAPSED, timeQueryID);


	// Issue draw calls
	for (size_t i = 0; i < vertexArrays.size(); ++i) {

		if (0 >= particles[i].lifetime)
		{
			continue;
		}

		VertexArrayObject vao = vertexArrays[i];
		vao.Bind();

		glUseProgram(shaderProgram);
		glUniformMatrix4fv(uMVPLoc, 1, GL_FALSE, glm::value_ptr(mvp));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	// End the queries
	//glEndQuery(GL_TIME_ELAPSED);

	// Retrieve query results
	//GLuint64 elapsedTime = 0;
	//glGetQueryObjectui64v(timeQueryID, GL_QUERY_RESULT, &elapsedTime);

	// Print the results
	//std::cout << "GPU Time for Draw Calls: " << elapsedTime / 1e6 << " ms" << std::endl;

	// Cleanup
	//glDeleteQueries(1, &timeQueryID);
}

void BaseParticleEmitter::SpawnParticle(Particle particle, int particleCount)
{
	particles.push_back(Particle(particle));

	VertexArrayObject vao = VertexArrayObject();
	vao.Bind();

	VertexBufferObject vbo = VertexBufferObject(this->squareVertices, sizeof(this->squareVertices));
	vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	float* particleData = new float[particleCount * 8];
	GetBufferData(&particle, particleCount, particleData);

	VertexBufferObject particlePropertiesBuffer = VertexBufferObject(particleData, sizeof(float) * 8);
	GLsizeiptr stride = sizeof(float) * 8;

	// Link the position attribute (layout 1)
	vao.LinkAttribute(particlePropertiesBuffer,
		1,                  // Attribute layout index
		3,                  // Number of components (x, y, z)
		GL_FLOAT,           // Data type
		GL_FALSE,           // Not normalized
		stride,             // Total size of one vertex
		(void*)0);          // Offset to position data

	glVertexAttribDivisor(1, 1);

	// Link the color attribute (layout 2) colour
	vao.LinkAttribute(particlePropertiesBuffer,
		2,
		4,
		GL_FLOAT,
		GL_FALSE,
		stride,
		(void*)(sizeof(float) * 3));

	glVertexAttribDivisor(2, 1);

	// Link the size attribute (layout 3) size
	vao.LinkAttribute(particlePropertiesBuffer,
		3,
		1,
		GL_FLOAT,
		GL_FALSE,
		stride,
		(void*)(sizeof(float) * 7));

	glVertexAttribDivisor(3, 1);

	//vbo.Bind();
	//GLint bufferSize = 0;
	//glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
	//std::cout << "Buffer Size: " << bufferSize << " bytes" << std::endl;

	vertexArrays.push_back(vao);
	vertexBuffers.push_back(vbo);
	particlePropertiesBuffers.push_back(particlePropertiesBuffer);

	delete[] particleData;
}

void BaseParticleEmitter::RemoveParticles(const std::vector<int>& particlesToRemove)
{
	// Copy and sort indices in descending order to avoid shifting issues when erasing
	std::vector<int> sortedIndices = particlesToRemove;
	std::sort(sortedIndices.rbegin(), sortedIndices.rend());

	for (int index : sortedIndices) {
		if (index >= 0 && index < vertexArrays.size()) {  // Ensure the index is valid
			// Delete particle resources
			vertexArrays[index].Delete();
			vertexBuffers[index].Delete();
			particlePropertiesBuffers[index].Delete();

			vertexArrays.erase(vertexArrays.begin() + index);
			vertexBuffers.erase(vertexBuffers.begin() + index);
			particlePropertiesBuffers.erase(particlePropertiesBuffers.begin() + index);

			particles.erase(particles.begin() + index);

			properties->activeParticleCount--;
		}
		else {
			// Handle invalid index, could log or assert here
			std::cerr << "Warning: Attempt to remove particle with invalid index: " << index << std::endl;
		}
	}
}


void BaseParticleEmitter::GetBufferData(const Particle* particles, int particleCount, float* outArray)
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

