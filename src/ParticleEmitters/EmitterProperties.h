#pragma once

#include <glm/glm.hpp>

struct EmitterProperties {
    float timeSinceLastEmission = 0.0f;
    float emissionInterval = 0.0f;
    unsigned int particlesPerSecond = 0;

    unsigned int particleCount = 0;
    unsigned int activeParticleCount = 0;

    float particleLifetime = 0.0f;
    float colour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float size = 0.0f;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    bool randomiseLifetime = false;
    bool randomiseSize = false;
    bool randomisePosition = false;
    bool randomiseVelocity = false;
    bool randomiseColour = false;

    float minLifetime = 0.0f;
    float maxLifetime = 0.0f;
    float minSize = 0.0f;
    float maxSize = 0.0f;
    glm::vec3 minPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 maxPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 minVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 maxVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
};