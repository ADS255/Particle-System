#include "BaseParticleEmitter.h"
#include "BaseParticleEmitter.h"
#include "BaseParticleEmitter.h"
#include "BaseParticleEmitter.h"
#include "BaseParticleEmitter.h"
#include "BaseParticleEmitter.h"
#include "BaseParticleEmitter.h"

BaseParticleEmitter::BaseParticleEmitter(unsigned int particleCount, float particleLifetime)
{
	modifiers = std::vector<ParticlePropertyModifier*>();

	float colour[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glm::vec3 position(0.0f,0.0f,0.0f);
	glm::vec3 velocity(0.0f, 0.0f, 0.0f);
	float size = 1.0f;

	for (size_t i = 0; i < particleCount; i++)
	{
		particles.push_back(Particle(colour, position, velocity, size,particleLifetime));
	}

	vertexArrays = std::vector<VertexArrayObject>();
	vertexBuffers = std::vector<VertexBufferObject>();
	particlePropertiesBuffers = std::vector<VertexBufferObject>();

	for (size_t i = 0; i < particleCount; ++i) {
		VertexArrayObject vao = VertexArrayObject();
		vao.Bind();

		VertexBufferObject vbo = VertexBufferObject(this->squareVertices, sizeof(this->squareVertices));
		vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		float* particleData = new float[8];
		particleData[0] = particles[i].position.x;
		particleData[1] = particles[i].position.y;
		particleData[2] = particles[i].position.z;
		particleData[3] = particles[i].colour[0];
		particleData[4] = particles[i].colour[1];
		particleData[5] = particles[i].colour[2];
		particleData[6] = particles[i].colour[3];
		particleData[7] = particles[i].size;

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

		// Link the color attribute (layout 3) size
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

BaseParticleEmitter::~BaseParticleEmitter()
{
}

void BaseParticleEmitter::Update(double deltaTime)
{
	for (size_t i = 0; i < particles.size(); ++i) {
		VertexBufferObject particlePropertiesBuffer = particlePropertiesBuffers[i];
		particles[i].lifetime -= deltaTime;

		for (size_t i = 0; i < modifiers.size(); i++)
		{
			modifiers[i]->ApplyModifier(particles[i],deltaTime);
		}
		glBindBuffer(GL_ARRAY_BUFFER, particlePropertiesBuffer.ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3, &particles[i].position);
	}
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
			std::cout << particles[i].lifetime << std::endl;

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
