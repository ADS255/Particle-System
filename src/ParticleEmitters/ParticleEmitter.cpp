#pragma once
#include "ParticleEmitter.h"

void ParticleEmitter::Editor() {

    ImGui::Begin("Particle System Editor");

    // Particle count
    ImGui::InputInt("Particle Count", (int*)&particleCount, 1, 10000);

    // Particle lifetime
    ImGui::InputFloat("Lifetime", &particleLifetime, 0.1f, 10.0f);

    // Particle color
    ImGui::ColorEdit4("Color", colour);

    // Particle size
    ImGui::InputFloat("Size", &size, 0.1f, 10.0f);

    // Particle position
    ImGui::InputFloat3("Position", &position[0]);

    // Particle velocity
    ImGui::InputFloat3("Velocity", &velocity[0]);

	if (ImGui::Button("Apply")) {
        Destroy();
        Initialise();
	}

	ImGui::End();
}