#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <random>

#include "opengl/GLUtils.h"
#include "opengl/VertexArrayObject.h"
#include "opengl/VertexBufferObject.h"

#include "Particle.h"
#include "ParticlePropertyModifier.h"

class ParticleEmitter
{
public:

    virtual ~ParticleEmitter() {}

    virtual void Update(double deltaTime) = 0;

    virtual void Render(glm::mat4 mvp) = 0;

    std::vector<ParticlePropertyModifier*> modifiers;

protected:

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

    unsigned int particleCount;
    float particleLifetime;

    GLuint shaderProgram;
    GLuint uMVPLoc;

};
