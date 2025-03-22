#pragma once

#include <glm/glm.hpp>
#include <cstring>

struct Particle
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec4 colour = glm::vec4(1.0f);
    float size = 1.0f;
    glm::vec3 velocity = glm::vec3(0.0f);
    float lifetime = 1.0f;
    float maxLifetime = 1.0f;

    Particle() {}

    Particle(const glm::vec4 colour, const glm::vec3& position, const glm::vec3& velocity, const float size, const float lifetime)
        : position(position), velocity(velocity), size(size), lifetime(lifetime)
    {
        maxLifetime = lifetime;

        for (int i = 0; i < 4; ++i) {
            this->colour[i] = colour[i];
        }
    }
};
