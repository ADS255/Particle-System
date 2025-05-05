#include "BaseParticleEmitter_3.h"

BaseParticleEmitter_3::BaseParticleEmitter_3() {
	vertexArrayObject = VertexArrayObject();
	vertexArrayObject.Bind();

	billboardVBO = VertexBufferObject(this->squareVertices, sizeof(this->squareVertices));
	vertexArrayObject.LinkAttribute(billboardVBO, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	vertexArrayObject.LinkAttribute(billboardVBO, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	unsigned int vertexShader = GLUtils::LoadShader("shaders/gpu_1.vert", GL_VERTEX_SHADER);
	unsigned int fragmentShader = GLUtils::LoadShader("shaders/baseParticleEmitter.frag", GL_FRAGMENT_SHADER);
	unsigned int manageComputeShader = GLUtils::LoadShader("shaders/particleManage_1.comp", GL_COMPUTE_SHADER);
	unsigned int updateComputeShader = GLUtils::LoadShader("shaders/particleUpdate_1.comp", GL_COMPUTE_SHADER);

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

	particleManagerComputeShaderProgram = glCreateProgram();
	glAttachShader(particleManagerComputeShaderProgram, manageComputeShader);
	glLinkProgram(particleManagerComputeShaderProgram);

	success;
	glGetProgramiv(particleManagerComputeShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(particleManagerComputeShaderProgram, 512, nullptr, infoLog);
		std::cerr << "Shader program linking error:\n" << infoLog << std::endl;
	}
	uActiveParticleOffset = glGetUniformLocation(particleManagerComputeShaderProgram, "uActiveParticleOffset");

	particleUpdateComputeShaderProgram = glCreateProgram();
	glAttachShader(particleUpdateComputeShaderProgram, updateComputeShader);
	glLinkProgram(particleUpdateComputeShaderProgram);

	success;
	glGetProgramiv(particleUpdateComputeShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(particleUpdateComputeShaderProgram, 512, nullptr, infoLog);
		std::cerr << "Shader program linking error:\n" << infoLog << std::endl;
	}

	uDeltaTime = glGetUniformLocation(particleUpdateComputeShaderProgram, "uDeltaTime");
}

BaseParticleEmitter_3::~BaseParticleEmitter_3()
{
	vertexArrayObject.Delete();
	billboardVBO.Delete();
	Destroy();
}

void BaseParticleEmitter_3::Destroy()
{
	particleDataVBO.Delete();
	particles.clear();
}

void BaseParticleEmitter_3::Reload()
{
	ParticleEmitter::Reload();

	float* propertyData = editorPropertiesToFloatArray();

	glDeleteBuffers(1, &particlePropertiesSSBO);
	glGenBuffers(1, &particlePropertiesSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particlePropertiesSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 24, propertyData, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particlePropertiesSSBO);

	glDeleteBuffers(1, &activeParticleCountAtomicBuffer);
	glGenBuffers(1, &activeParticleCountAtomicBuffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, activeParticleCountAtomicBuffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), &activeParticleCount, GL_DYNAMIC_DRAW);

	//---
	glDeleteBuffers(1, &particlePosSSBO);
	glGenBuffers(1, &particlePosSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particlePosSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 3 * particleCount, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, particlePosSSBO);


	glDeleteBuffers(1, &particleColourSSBO);
	glGenBuffers(1, &particleColourSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleColourSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 4 * particleCount, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, particleColourSSBO);

	glDeleteBuffers(1, &particleSizeSSBO);
	glGenBuffers(1, &particleSizeSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSizeSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * particleCount, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, particleSizeSSBO);

	glDeleteBuffers(1, &particleVelocitySSBO);
	glGenBuffers(1, &particleVelocitySSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleVelocitySSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 3 * particleCount, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, particleVelocitySSBO);

	glDeleteBuffers(1, &particleLifetimeSBBO);
	glGenBuffers(1, &particleLifetimeSBBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleLifetimeSBBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * particleCount, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, particleLifetimeSBBO);

	glDeleteBuffers(1, &particleMaxLifetimeSSBO);
	glGenBuffers(1, &particleMaxLifetimeSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleMaxLifetimeSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * particleCount, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, particleMaxLifetimeSSBO);


	/*
	glm::vec3 testPos(0.0f, 0.0f, 0.0f);
	glm::vec4 testColor(1.0f, 0.0f, 0.0f, 1.0f); // red
	float testSize = 10.0f; // pixels if you're using gl_PointSize
	glm::vec3 testVelocity(0.0f, 0.0f, 0.0f);
	float testLifetime = 5.0f;
	float testMaxLifetime = 5.0f;

	// Write to GPU buffers (only for first particle)
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particlePosSSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec3), &testPos);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleColourSSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4), &testColor);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSizeSSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float), &testSize);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleVelocitySSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec3), &testVelocity);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleLifetimeSBBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float), &testLifetime);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleMaxLifetimeSSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float), &testMaxLifetime);
	*/
}

void BaseParticleEmitter_3::Update(double deltaTime, glm::vec3 cameraPos)
{
	if (particleCount <= 0)
	{
		return;
	}


	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particlePropertiesSSBO);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 1, activeParticleCountAtomicBuffer);
	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, particlePosSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, particleColourSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, particleSizeSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, particleVelocitySSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, particleLifetimeSBBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, particleMaxLifetimeSSBO);


	timeSinceLastEmission += deltaTime;

	if (timeSinceLastEmission >= emissionInterval && activeParticleCount < particleCount)
	{
		unsigned int particlesToEmit = ParticlesToEmitCount();

		if (particlesToEmit > 0)
		{
			glUseProgram(particleManagerComputeShaderProgram);
			glUniform1ui(uActiveParticleOffset, activeParticleCount + particlesToEmit);

			//glDispatchCompute(particleCount, 1, 1);
			glDispatchCompute(std::ceil(particleCount / 64), 1, 1);
			glMemoryBarrier(GL_ALL_BARRIER_BITS);

			activeParticleCount += particlesToEmit;
			timeSinceLastEmission = 0;


			glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &activeParticleCount);
		}
	
	
	}

	//std::cout << activeParticleCount << std::endl;

	if (activeParticleCount > 0)
	{
		float deltaTimeF = static_cast<float>(deltaTime);
		glUseProgram(particleUpdateComputeShaderProgram);
		glUniform1f(uDeltaTime, deltaTimeF);
		//glDispatchCompute(particleCount, 1, 1);
		glDispatchCompute(std::ceil(particleCount / 64), 1, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, activeParticleCountAtomicBuffer);
		GLuint* counterValue = (GLuint*)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_READ_ONLY);
		activeParticleCount = *counterValue;
		glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
	}

	//std::cout << activeParticleCount << std::endl;
}

void BaseParticleEmitter_3::Render(glm::mat4 view, glm::mat4 proj)
{
	if (particleCount <= 0 || activeParticleCount <= 0)
	{
		return;
	}

	using Clock = std::chrono::high_resolution_clock;
	auto frame_start = Clock::now();

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, particlePosSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, particleColourSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, particleSizeSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, particleLifetimeSBBO);
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

	/*
	GLuint query;
	glGenQueries(1, &query);

	// Begin query
	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	// Your draw call
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, activeParticleCount);

	// End query
	glEndQuery(GL_PRIMITIVES_GENERATED);

	// Get the result
	GLuint primitivesGenerated = 0;
	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);

	printf("Primitives generated: %u\n", primitivesGenerated);
	*/



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

void BaseParticleEmitter_3::SpawnParticle(Particle particle, int particleCount)
{
	particles.push_back(Particle(particle));
}

void BaseParticleEmitter_3::RemoveParticles(const std::vector<int>& particlesToRemove)
{
	std::vector<int> sortedIndices = particlesToRemove;
	std::sort(sortedIndices.rbegin(), sortedIndices.rend());

	for (int index : sortedIndices) {
		particles.erase(particles.begin() + index);
		activeParticleCount--;
	}
}


float* BaseParticleEmitter_3::editorPropertiesToFloatArray()
{
	float* properties = new float[24];
	int offset = 0;

	for (IParticlePropertyModifier* modifier : modifiers) {

		std::vector<float> propertyVec = modifier->AsFloats();

		std::cout << "Property:" << std::endl;

		for (size_t i = 0; i < propertyVec.size(); i++)
		{
			std::cout << propertyVec[i] << std::endl;
			properties[offset] = propertyVec[i];
			offset++;
		}

	}

	return properties;
}

void BaseParticleEmitter_3::GetBufferData(const Particle* particles, int particleCount, float* outArray)
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

double BaseParticleEmitter_3::GetUpdateTime()
{
	return updateTime;
}

double BaseParticleEmitter_3::GetRenderTime()
{
	return renderTime;
}

unsigned int BaseParticleEmitter_3::GetActiveParticleCount()
{
	return activeParticleCount;

}

unsigned int BaseParticleEmitter_3::GetParticleGPUSizeBytes()
{
	return sizeof(float) * 8;
}

unsigned int BaseParticleEmitter_3::GetTotalParticlesGPUSizeBytes()
{
	return activeParticleCount * GetParticleGPUSizeBytes();
}

unsigned int BaseParticleEmitter_3::GetTotalDataTransferBytes()
{
	return activeParticleCount * GetParticleGPUSizeBytes();
}

unsigned int BaseParticleEmitter_3::GetTotalDrawCalls()
{
	return frameDrawCallsCount;
}


