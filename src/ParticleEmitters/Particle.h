#pragma once

#include <glm/glm.hpp>
#include <cstring>

struct Particle
{
    glm::vec3 position;
    float colour[4];
    float size;
    glm::vec3 velocity;
    float lifetime;

    Particle(){}

    Particle(const float colour[4], const glm::vec3& position, const glm::vec3& velocity, const float size, const float lifetime)
        : position(position), velocity(velocity), size(size), lifetime(lifetime)
    {
        for (int i = 0; i < 4; ++i) {
            this->colour[i] = colour[i];
        }
    }
};
