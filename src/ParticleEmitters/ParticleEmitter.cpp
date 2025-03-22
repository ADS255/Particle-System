#include "ParticleEmitter.h"
#include "io/Serialiser.h"

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
	emissionInterval = 1.0f / particlesPerSecond;
	int particleSpawnCount = static_cast<unsigned int>(timeSinceLastEmission / emissionInterval);

	if (activeParticleCount + particleSpawnCount > particleCount)
	{
		particleSpawnCount = particleCount - activeParticleCount;
	}

	return particleSpawnCount;
}

void ParticleEmitter::Editor() {

	ImGui::Begin("Particle System Editor");

	ImGui::InputInt("Particles To Emit Per Second", (int*)&particlesPerSecond);

	ImGui::InputInt("Particle Count", (int*)&particleCount);

	for (size_t i = 0; i < modifiers.size(); ++i) {
		IParticlePropertyModifier* modifier = modifiers[i];

		modifier->Editor();
	}

	if (ImGui::Button("Apply")) {
		Destroy();
		Initialise();
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
	ImGui::InputText("Save Path", &savePath[0], savePath.capacity());

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
	Serialiser serialiser = Serialiser();
	serialiser.SerialiseParticleEmitter(*this);
}

void ParticleEmitter::LoadParticleSystemConfig(std::string path)
{
	Serialiser serialiser = Serialiser();
	serialiser.DeserialiseParticleEmitter(*this);
	Destroy();
	Initialise();
}
