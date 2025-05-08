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

	std::vector<float> zeroed(particleCount, 0.0f);

	glDeleteBuffers(1, &particleLifetimeSBBO);
	glGenBuffers(1, &particleLifetimeSBBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleLifetimeSBBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * particleCount, zeroed.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, particleLifetimeSBBO);

	glDeleteBuffers(1, &particleMaxLifetimeSSBO);
	glGenBuffers(1, &particleMaxLifetimeSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleMaxLifetimeSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * particleCount, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, particleMaxLifetimeSSBO);
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

	float groupsXFloat = std::ceil(static_cast<float>(particleCount) / 64);
	unsigned int groupsX = static_cast<unsigned int>(groupsXFloat);

	if (timeSinceLastEmission >= emissionInterval && activeParticleCount < particleCount)
	{
		unsigned int particlesToEmit = ParticlesToEmitCount();

		if (particlesToEmit > 0)
		{
			glUseProgram(particleManagerComputeShaderProgram);
			glUniform1ui(uActiveParticleOffset, activeParticleCount + particlesToEmit);

			glDispatchCompute(groupsX, 1, 1);
			glMemoryBarrier(GL_ALL_BARRIER_BITS);

			activeParticleCount += particlesToEmit;
			timeSinceLastEmission = 0;


			glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &activeParticleCount);
		}
	
	
	}

	if (activeParticleCount > 0)
	{
		float deltaTimeF = static_cast<float>(deltaTime);
		glUseProgram(particleUpdateComputeShaderProgram);
		glUniform1f(uDeltaTime, deltaTimeF);
		//glDispatchCompute(particleCount, 1, 1);
		glDispatchCompute(groupsX, 1, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, activeParticleCountAtomicBuffer);
		GLuint* counterValue = (GLuint*)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_READ_ONLY);
		activeParticleCount = *counterValue;
		glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
	}
}

void BaseParticleEmitter_3::Render(glm::mat4 view, glm::mat4 proj)
{
	if (particleCount <= 0 || activeParticleCount <= 0)
	{
		return;
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, particlePosSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, particleColourSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, particleSizeSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, particleLifetimeSBBO);

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

		for (size_t i = 0; i < propertyVec.size(); i++)
		{
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


