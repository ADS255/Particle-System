#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() {

    modifiers = std::vector<IParticlePropertyModifier*>();

    modifiers.push_back(new LifetimeModifier());
    modifiers.push_back(new SizeModifier());
    modifiers.push_back(new PositionModifier());
    modifiers.push_back(new VelocityModifier());
    modifiers.push_back(new ColourModifier());

    overLifetimeModifiers = std::vector<IParticlePropertyOverLifetimeModifer*>();

    overLifetimeModifiers.push_back(new ColourOverLifetimeModifer());
    overLifetimeModifiers.push_back(new SizeOverLifetimeModifier());
}

unsigned int ParticleEmitter::ParticlesToEmitCount()
{
    if (properties->particlesPerSecond == 0)
    {
        return 0;
    }

    properties->emissionInterval = 1.0f / properties->particlesPerSecond;
    int particleSpawnCount = static_cast<unsigned int>(properties->timeSinceLastEmission / properties->emissionInterval);

    if (properties->activeParticleCount + particleSpawnCount > properties->particleCount)
    {
        particleSpawnCount = properties->particleCount - properties->activeParticleCount;
    }

    return particleSpawnCount;
}

void ParticleEmitter::Editor() {

    ImGui::Begin("Particle System Editor");

    ImGui::InputInt("Particles To Emit Per Second", (int*)&editorProperties.particlesPerSecond);

    ImGui::InputInt("Particle Count", (int*)&editorProperties.particleCount);

    for (size_t i = 0; i < modifiers.size(); ++i) {
        IParticlePropertyModifier* modifier = modifiers[i];

        modifier->Editor();
    }

	if (ImGui::Button("Apply")) {
        Destroy();
        Initialise();
        properties = editorProperties;
	}

    ImGui::Separator();

    for (size_t i = 0; i < overLifetimeModifiers.size(); ++i) {
        IParticlePropertyOverLifetimeModifer* modifier = overLifetimeModifiers[i];
        modifier->Editor();
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
