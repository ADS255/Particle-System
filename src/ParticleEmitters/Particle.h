#pragma once

#include <glm/glm.hpp>
#include <cstring>

struct Particle
{
    float colour[4];
    glm::vec3 position;
    glm::vec3 velocity;
    float size;
    float lifetime;

    Particle(const float colour[4], const glm::vec3& position, const glm::vec3& velocity, const float size, const float lifetime)
        : position(position), velocity(velocity), size(size), lifetime(lifetime)
    {
        for (int i = 0; i < 4; ++i) {
            this->colour[i] = colour[i];
        }
    }
};
