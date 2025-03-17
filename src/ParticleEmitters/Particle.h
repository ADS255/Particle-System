#pragma once

#include <glm/glm.hpp>
#include <cstring>

struct Particle
{
    glm::vec3 position;
    glm::vec4 colour;
    float size;
    glm::vec3 velocity;
    float lifetime;
    float maxLifetime;

    Particle(const glm::vec4 colour, const glm::vec3& position, const glm::vec3& velocity, const float size, const float lifetime)
        : position(position), velocity(velocity), size(size), lifetime(lifetime)
    {
        maxLifetime = lifetime;

        for (int i = 0; i < 4; ++i) {
            this->colour[i] = colour[i];
        }
    }
};
