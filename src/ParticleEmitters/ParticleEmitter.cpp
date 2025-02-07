#include "ParticleEmitter.h"
#pragma once
#include "ParticleEmitter.h"

void ParticleEmitter::Editor() {

    ImGui::Begin("Particle System Editor");

    ImGui::InputInt("Particle Count", (int*)&particleCount, 1, 10000);

    // Particle lifetime
    ImGui::InputFloat("Lifetime", &particleLifetime, 0.1f, 10.0f);
    ImGui::Checkbox("Randomize Lifetime", &randomiseLifetime);
    if (randomiseLifetime) {
        ImGui::InputFloat("Min Lifetime", &minLifetime);
        ImGui::InputFloat("Max Lifetime", &maxLifetime);
    }

    // Particle color
    ImGui::ColorEdit4("Colour", colour);
    ImGui::Checkbox("Colour Over Lifetime", &randomiseColour);

    // Particle size
    ImGui::InputFloat("Size", &size, 0.1f, 10.0f);
    ImGui::Checkbox("Randomize Size", &randomiseSize);
    if (randomiseSize) {
        ImGui::InputFloat("Min Size", &minSize);
        ImGui::InputFloat("Max Size", &maxSize);
    }

    // Particle position
    ImGui::InputFloat3("Position", &position[0]);
    ImGui::Checkbox("Randomize Position", &randomisePosition);
    if (randomisePosition) {
        ImGui::InputFloat("Min Position X", &minPosition[0]);
        ImGui::InputFloat("Max Position X", &maxPosition[0]);
        ImGui::InputFloat("Min Position Y", &minPosition[1]);
        ImGui::InputFloat("Max Position Y", &maxPosition[1]);
        ImGui::InputFloat("Min Position Z", &minPosition[2]);
        ImGui::InputFloat("Max Position Z", &maxPosition[2]);
    }

    // Particle velocity
    ImGui::InputFloat3("Velocity", &velocity[0]);
    ImGui::Checkbox("Randomize Velocity", &randomiseVelocity);
    if (randomiseVelocity) {
        ImGui::InputFloat("Min Velocity X", &minVelocity[0]);
        ImGui::InputFloat("Max Velocity X", &maxVelocity[0]);
        ImGui::InputFloat("Min Velocity Y", &minVelocity[1]);
        ImGui::InputFloat("Max Velocity Y", &maxVelocity[1]);
        ImGui::InputFloat("Min Velocity Z", &minVelocity[2]);
        ImGui::InputFloat("Max Velocity Z", &maxVelocity[2]);
    }

	if (ImGui::Button("Apply")) {
        Destroy();
        Initialise();
	}

	ImGui::End();
}

float ParticleEmitter::RandomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}
