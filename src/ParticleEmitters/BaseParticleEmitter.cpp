#include "BaseParticleEmitter.h"
#include "BaseParticleEmitter.h"
#include "BaseParticleEmitter.h"
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

	Particle particle(colour, position, velocity, size, particleLifetime);

	for (size_t i = 0; i < particleCount; ++i) {
		SpawnParticle(particle, 1);
	}

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

		for (size_t j = 0; j < modifiers.size(); j++)
		{
			modifiers[j]->ApplyModifier(particles[i], deltaTime);
		}

		GetBufferData(&particles[i], 1, particleData);
		glBindBuffer(GL_ARRAY_BUFFER, particlePropertiesBuffer.ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 8, particleData);
	}

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

void BaseParticleEmitter::RemoveParticle(int particleIndex)
{
	vertexArrays[particleIndex].Delete();
	vertexBuffers[particleIndex].Delete();
	particlePropertiesBuffers[particleIndex].Delete();

	vertexArrays.erase(vertexArrays.begin() + particleIndex);
	vertexBuffers.erase(vertexBuffers.begin() + particleIndex);
	particlePropertiesBuffers.erase(particlePropertiesBuffers.begin() + particleIndex);
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

