#include "ParticleEmitter.h"
#include "ParticleEmitter.h"
#include "ParticleEmitter.h"
#include "ParticleEmitter.h"
#include "ParticleEmitter.h"
#include "ParticleEmitter.h"
#pragma once
#include "ParticleEmitter.h"

unsigned int ParticleEmitter::ParticlesToEmitCount()
{
    if (particlesPerSecond == 0)
    {
        return 0;
    }

    emissionInterval = 1.0f / particlesPerSecond;
    return static_cast<unsigned int>(timeSinceLastEmission / emissionInterval);
}

void ParticleEmitter::Editor() {

    ImGui::Begin("Particle System Editor");

    ImGui::InputInt("Particles To Emit Per Second", (int*)&particlesPerSecond);

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

    ImGui::Separator();

    static std::string savePath = "assets/ps_config.json";

    if (ImGui::Button("Save Config")) {
        this->SaveParticleSystemConfig(savePath);
    }
    ImGui::InputText("Save Path", &savePath[0],savePath.capacity());

    ImGui::Separator();

    static std::string loadPath = "assets/ps_config.json";

    if (ImGui::Button("Load Config")) {
        this->LoadParticleSystemConfig(loadPath);
    }
    ImGui::InputText("Load Path", &loadPath[0], loadPath.capacity());

	ImGui::End();
}

void ParticleEmitter::SaveParticleSystemConfig(std::string path)
{
    nlohmann::json config;

    config["particleCount"] = particleCount;
    config["particleLifetime"] = particleLifetime;
    config["colour"] = { colour[0], colour[1], colour[2], colour[3] };
    config["size"] = size;
    config["position"] = { position.x, position.y, position.z };
    config["velocity"] = { velocity.x, velocity.y, velocity.z };

    config["randomiseLifetime"] = randomiseLifetime;
    config["randomiseSize"] = randomiseSize;
    config["randomisePosition"] = randomisePosition;
    config["randomiseVelocity"] = randomiseVelocity;
    config["randomiseColour"] = randomiseColour;

    config["minLifetime"] = minLifetime;
    config["maxLifetime"] = maxLifetime;
    config["minSize"] = minSize;
    config["maxSize"] = maxSize;
    config["minPosition"] = { minPosition.x, minPosition.y, minPosition.z };
    config["maxPosition"] = { maxPosition.x, maxPosition.y, maxPosition.z };
    config["minVelocity"] = { minVelocity.x, minVelocity.y, minVelocity.z };
    config["maxVelocity"] = { maxVelocity.x, maxVelocity.y, maxVelocity.z };

    std::ofstream file(path);

    if (file.is_open()) {

        file << config.dump(4);
        file.close();
    }
    else {
        // Handle error if file cannot be opened
        std::cerr << "Could not open file for writing!" << std::endl;
    }
}

void ParticleEmitter::LoadParticleSystemConfig(std::string path)
{
    std::ifstream file(path);
    if (file.is_open()) {
        nlohmann::json config;
        file >> config;
        file.close();

        // Deserialize JSON into member variables
        particleCount = config["particleCount"].get<unsigned int>();
        particleLifetime = config["particleLifetime"].get<float>();
        std::vector<float> colourArray = config["colour"].get<std::vector<float>>();
        for (int i = 0; i < 4; ++i) {
            colour[i] = colourArray[i];
        }
        size = config["size"].get<float>();
        position = glm::vec3(config["position"][0].get<float>(), config["position"][1].get<float>(), config["position"][2].get<float>());
        velocity = glm::vec3(config["velocity"][0].get<float>(), config["velocity"][1].get<float>(), config["velocity"][2].get<float>());

        randomiseLifetime = config["randomiseLifetime"].get<bool>();
        randomiseSize = config["randomiseSize"].get<bool>();
        randomisePosition = config["randomisePosition"].get<bool>();
        randomiseVelocity = config["randomiseVelocity"].get<bool>();
        randomiseColour = config["randomiseColour"].get<bool>();

        minLifetime = config["minLifetime"].get<float>();
        maxLifetime = config["maxLifetime"].get<float>();
        minSize = config["minSize"].get<float>();
        maxSize = config["maxSize"].get<float>();
        minPosition = glm::vec3(config["minPosition"][0].get<float>(), config["minPosition"][1].get<float>(), config["minPosition"][2].get<float>());
        maxPosition = glm::vec3(config["maxPosition"][0].get<float>(), config["maxPosition"][1].get<float>(), config["maxPosition"][2].get<float>());
        minVelocity = glm::vec3(config["minVelocity"][0].get<float>(), config["minVelocity"][1].get<float>(), config["minVelocity"][2].get<float>());
        maxVelocity = glm::vec3(config["maxVelocity"][0].get<float>(), config["maxVelocity"][1].get<float>(), config["maxVelocity"][2].get<float>());
    }
    else {
        std::cerr << "Could not open file for reading!" << std::endl;
    }
}

float ParticleEmitter::RandomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}
