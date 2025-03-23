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

	LoadSetTexture();
}

void ParticleEmitter::LoadSetTexture()
{
	stbi_set_flip_vertically_on_load(1);
	int w;
	int h;
	int comp;
	unsigned char* image = NULL;

	if (!texturePath.empty())
	{
		image = stbi_load(texturePath.c_str(), &w, &h, &comp, 0);
	}
	else
	{
		image = stbi_load(defaultTexturePath.c_str(), &w, &h, &comp, 0);
	}

	if (!image) {
		std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
		exit;
	}

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	GLenum format = (comp == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);
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
		Reload();
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

void ParticleEmitter::Reload()
{
	activeParticleCount = 0;
	timeSinceLastEmission = 0.0f;
	emissionInterval = 0.0f;

	LoadSetTexture();
	Destroy();
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
	Reload();
}
