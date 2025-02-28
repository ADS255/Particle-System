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
    if (properties->particlesPerSecond == 0)
    {
        return 0;
    }

    properties->emissionInterval = 1.0f / properties->particlesPerSecond;
    return static_cast<unsigned int>(properties->timeSinceLastEmission / properties->emissionInterval);
}

void ParticleEmitter::Editor() {

    ImGui::Begin("Particle System Editor");

    ImGui::InputInt("Particles To Emit Per Second", (int*)&editorProperties.particlesPerSecond);

    ImGui::InputInt("Particle Count", (int*)&editorProperties.particleCount, 1, 10000);

    // Particle lifetime
    ImGui::InputFloat("Lifetime", &editorProperties.particleLifetime, 0.1f, 10.0f);
    ImGui::Checkbox("Randomize Lifetime", &editorProperties.randomiseLifetime);
    if (editorProperties.randomiseLifetime) {
        ImGui::InputFloat("Min Lifetime", &editorProperties.minLifetime);
        ImGui::InputFloat("Max Lifetime", &editorProperties.maxLifetime);
    }

    // Particle color
    ImGui::ColorEdit4("Colour", editorProperties.colour);
    ImGui::Checkbox("Colour Over Lifetime", &editorProperties.randomiseColour);

    // Particle size
    ImGui::InputFloat("Size", &editorProperties.size, 0.1f, 10.0f);
    ImGui::Checkbox("Randomize Size", &editorProperties.randomiseSize);
    if (editorProperties.randomiseSize) {
        ImGui::InputFloat("Min Size", &editorProperties.minSize);
        ImGui::InputFloat("Max Size", &editorProperties.maxSize);
    }

    // Particle position
    ImGui::InputFloat3("Position", &editorProperties.position[0]);
    ImGui::Checkbox("Randomize Position", &editorProperties.randomisePosition);
    if (editorProperties.randomisePosition) {
        ImGui::InputFloat("Min Position X", &editorProperties.minPosition[0]);
        ImGui::InputFloat("Max Position X", &editorProperties.maxPosition[0]);
        ImGui::InputFloat("Min Position Y", &editorProperties.minPosition[1]);
        ImGui::InputFloat("Max Position Y", &editorProperties.maxPosition[1]);
        ImGui::InputFloat("Min Position Z", &editorProperties.minPosition[2]);
        ImGui::InputFloat("Max Position Z", &editorProperties.maxPosition[2]);
    }

    // Particle velocity
    ImGui::InputFloat3("Velocity", &editorProperties.velocity[0]);
    ImGui::Checkbox("Randomize Velocity", &editorProperties.randomiseVelocity);
    if (editorProperties.randomiseVelocity) {
        ImGui::InputFloat("Min Velocity X", &editorProperties.minVelocity[0]);
        ImGui::InputFloat("Max Velocity X", &editorProperties.maxVelocity[0]);
        ImGui::InputFloat("Min Velocity Y", &editorProperties.minVelocity[1]);
        ImGui::InputFloat("Max Velocity Y", &editorProperties.maxVelocity[1]);
        ImGui::InputFloat("Min Velocity Z", &editorProperties.minVelocity[2]);
        ImGui::InputFloat("Max Velocity Z", &editorProperties.maxVelocity[2]);
    }

	if (ImGui::Button("Apply")) {
        Destroy();
        Initialise();
        properties = editorProperties;
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

    config["particleCount"] = editorProperties.particleCount;
    config["particleLifetime"] = editorProperties.particleLifetime;
    config["colour"] = { editorProperties.colour[0], editorProperties.colour[1], editorProperties.colour[2], editorProperties.colour[3] };
    config["size"] = editorProperties.size;
    config["position"] = { editorProperties.position.x, editorProperties.position.y, editorProperties.position.z };
    config["velocity"] = { editorProperties.velocity.x, editorProperties.velocity.y, editorProperties.velocity.z };

    config["randomiseLifetime"] = editorProperties.randomiseLifetime;
    config["randomiseSize"] = editorProperties.randomiseSize;
    config["randomisePosition"] = editorProperties.randomisePosition;
    config["randomiseVelocity"] = editorProperties.randomiseVelocity;
    config["randomiseColour"] = editorProperties.randomiseColour;

    config["minLifetime"] = editorProperties.minLifetime;
    config["maxLifetime"] = editorProperties.maxLifetime;
    config["minSize"] = editorProperties.minSize;
    config["maxSize"] = editorProperties.maxSize;
    config["minPosition"] = { editorProperties.minPosition.x, editorProperties.minPosition.y, editorProperties.minPosition.z };
    config["maxPosition"] = { editorProperties.maxPosition.x, editorProperties.maxPosition.y, editorProperties.maxPosition.z };
    config["minVelocity"] = { editorProperties.minVelocity.x, editorProperties.minVelocity.y, editorProperties.minVelocity.z };
    config["maxVelocity"] = { editorProperties.maxVelocity.x, editorProperties.maxVelocity.y, editorProperties.maxVelocity.z };

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
        editorProperties.particleCount = config["particleCount"].get<unsigned int>();
        editorProperties.particleLifetime = config["particleLifetime"].get<float>();
        std::vector<float> colourArray = config["colour"].get<std::vector<float>>();
        for (int i = 0; i < 4; ++i) {
            editorProperties.colour[i] = colourArray[i];
        }
        editorProperties.size = config["size"].get<float>();
        editorProperties.position = glm::vec3(config["position"][0].get<float>(), config["position"][1].get<float>(), config["position"][2].get<float>());
        editorProperties.velocity = glm::vec3(config["velocity"][0].get<float>(), config["velocity"][1].get<float>(), config["velocity"][2].get<float>());

        editorProperties.randomiseLifetime = config["randomiseLifetime"].get<bool>();
        editorProperties.randomiseSize = config["randomiseSize"].get<bool>();
        editorProperties.randomisePosition = config["randomisePosition"].get<bool>();
        editorProperties.randomiseVelocity = config["randomiseVelocity"].get<bool>();
        editorProperties.randomiseColour = config["randomiseColour"].get<bool>();

        editorProperties.minLifetime = config["minLifetime"].get<float>();
        editorProperties.maxLifetime = config["maxLifetime"].get<float>();
        editorProperties.minSize = config["minSize"].get<float>();
        editorProperties.maxSize = config["maxSize"].get<float>();
        editorProperties.minPosition = glm::vec3(config["minPosition"][0].get<float>(), config["minPosition"][1].get<float>(), config["minPosition"][2].get<float>());
        editorProperties.maxPosition = glm::vec3(config["maxPosition"][0].get<float>(), config["maxPosition"][1].get<float>(), config["maxPosition"][2].get<float>());
        editorProperties.minVelocity = glm::vec3(config["minVelocity"][0].get<float>(), config["minVelocity"][1].get<float>(), config["minVelocity"][2].get<float>());
        editorProperties.maxVelocity = glm::vec3(config["maxVelocity"][0].get<float>(), config["maxVelocity"][1].get<float>(), config["maxVelocity"][2].get<float>());
    }
    else {
        std::cerr << "Could not open file for reading!" << std::endl;
    }
}

float ParticleEmitter::RandomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}
