#include "BaseParticleEmitter.h"

BaseParticleEmitter::BaseParticleEmitter() {}

BaseParticleEmitter::~BaseParticleEmitter()
{
	Destroy();
}

void BaseParticleEmitter::Initialise()
{
	vertexArrays = std::vector<VertexArrayObject>();
	vertexBuffers = std::vector<VertexBufferObject>();
	particlePropertiesBuffers = std::vector<VertexBufferObject>();
	renderOrderIndices = std::vector<int>();

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

	stbi_set_flip_vertically_on_load(1);
	int w;
	int h;
	int comp;
	unsigned char* image = stbi_load("assets/textures/cloud.png", &w, &h, &comp, 0);

	if (!image) {
		std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
		exit;
	}

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	GLenum format = (comp == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);
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

void BaseParticleEmitter::Update(double deltaTime, glm::vec3 cameraPos)
{
	using Clock = std::chrono::high_resolution_clock;
	auto frame_start = Clock::now();

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

			for (size_t j = 0; j < modifiers.size(); j++)
			{
				modifiers[j]->ApplyModifier(particle, deltaTime);
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

		for (size_t j = 0; j < overLifetimeModifiers.size(); j++)
		{
			IParticlePropertyOverLifetimeModifer* modifier = overLifetimeModifiers[j];

			if (!modifier->enabled)
			{
				continue;
			}

			modifier->ApplyModifier(particles[i], deltaTime);
		}

		/*
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
		*/


		GetBufferData(&particles[i], 1, particleData);
		glBindBuffer(GL_ARRAY_BUFFER, particlePropertiesBuffer.ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 8, particleData);
	}

	RemoveParticles(particlesToDelete);
	delete[] particleData;

	/*
	renderOrderIndices.clear();
	for (size_t i = 0; i < particles.size(); i++)
	{
		renderOrderIndices.push_back(i);
	}

	std::sort(renderOrderIndices.begin(), renderOrderIndices.end(), [this, &cameraPos](int a, int b) {
	
		float distanceSquaredA = glm::dot(this->particles[a].position - cameraPos, this->particles[a].position - cameraPos); // Squared distance for a
		float distanceSquaredB = glm::dot(this->particles[b].position - cameraPos, this->particles[b].position - cameraPos); // Squared distance for b
		return distanceSquaredA > distanceSquaredB; // Compare squared distances
	});

	*/


	auto frame_end = Clock::now();
	updateTime = std::chrono::duration<double, std::milli>(frame_end - frame_start).count();
}

void BaseParticleEmitter::Render(glm::mat4 view, glm::mat4 proj)
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



	frameDrawCallsCount = 0;
	// Issue draw calls
	for (size_t i = 0; i < vertexArrays.size(); ++i) {

		if (0 >= particles[i].lifetime)
		{
			continue;
		}

		//int vaoIndex = renderOrderIndices[i];

		VertexArrayObject vao = vertexArrays[i];
		vao.Bind();

		glUseProgram(shaderProgram);
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform1i(uTexSlot, 0);

		glBindTexture(GL_TEXTURE_2D, texture);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		frameDrawCallsCount++;
	}

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

void BaseParticleEmitter::SpawnParticle(Particle particle, int particleCount)
{
	particles.push_back(Particle(particle));

	VertexArrayObject vao = VertexArrayObject();
	vao.Bind();

	VertexBufferObject vbo = VertexBufferObject(this->squareVertices, sizeof(this->squareVertices));
	vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	vao.LinkAttribute(vbo, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	float* particleData = new float[particleCount * 8];
	GetBufferData(&particle, particleCount, particleData);

	VertexBufferObject particlePropertiesBuffer = VertexBufferObject(particleData, sizeof(float) * 8);
	GLsizeiptr stride = sizeof(float) * 8;

	// Link the position attribute (layout 1)
	vao.LinkAttribute(particlePropertiesBuffer,
		2,                  // Attribute layout index
		3,                  // Number of components (x, y, z)
		GL_FLOAT,           // Data type
		GL_FALSE,           // Not normalized
		stride,             // Total size of one vertex
		(void*)0);          // Offset to position data

	glVertexAttribDivisor(2, 1);

	// Link the color attribute (layout 2) colour
	vao.LinkAttribute(particlePropertiesBuffer,
		3,
		4,
		GL_FLOAT,
		GL_FALSE,
		stride,
		(void*)(sizeof(float) * 3));

	glVertexAttribDivisor(3, 1);

	// Link the size attribute (layout 3) size
	vao.LinkAttribute(particlePropertiesBuffer,
		4,
		1,
		GL_FLOAT,
		GL_FALSE,
		stride,
		(void*)(sizeof(float) * 7));

	glVertexAttribDivisor(4, 1);

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

double BaseParticleEmitter::GetUpdateTime()
{
	return updateTime;
}

double BaseParticleEmitter::GetRenderTime()
{
	return renderTime;
}

unsigned int BaseParticleEmitter::GetActiveParticleCount()
{
	if (properties) {
		return properties->activeParticleCount;
	}

	return 0;
}

unsigned int BaseParticleEmitter::GetParticleGPUSizeBytes()
{
	return sizeof(float) * 8;
}

unsigned int BaseParticleEmitter::GetTotalParticlesGPUSizeBytes()
{
	if (properties) {
		return properties->activeParticleCount * GetParticleGPUSizeBytes();
	}
	return 0;
}

unsigned int BaseParticleEmitter::GetTotalDataTransferBytes()
{
	if (properties) {
		return properties->activeParticleCount * GetParticleGPUSizeBytes();
	}
	return 0;
}

unsigned int BaseParticleEmitter::GetTotalDrawCalls()
{
	return frameDrawCallsCount;
}

