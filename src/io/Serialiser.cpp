#include "Serialiser.h"

Serialiser::Serialiser()
{
}

Serialiser::~Serialiser()
{
}

void Serialiser::SerialiseParticleEmitter(ParticleEmitter& emitter)
{
	nlohmann::json json;

	json["texture_path"] = emitter.texturePath;
	json["emission_rate"] = emitter.particlesPerSecond;
	json["max_count"] = emitter.particleCount;

	json["property_modifiers"] = nlohmann::json::array();
    for (size_t i = 0; i < emitter.modifiers.size(); ++i) {
        IParticlePropertyModifier* modifier = emitter.modifiers[i];
		modifier->Serialise(json);
    }

	FileHandler::SaveFileJSON(json);
}

void Serialiser::DeserialiseParticleEmitter(ParticleEmitter& emitter)
{
	nlohmann::json json = FileHandler::LoadFileJSON();

	emitter.texturePath = json["texture_path"].get<std::string>();
	emitter.particlesPerSecond = json["emission_rate"].get<unsigned int>();
	emitter.editorParticleCount = json["max_count"].get<unsigned int>();

	for (size_t i = 0; i < emitter.modifiers.size(); ++i) {
		IParticlePropertyModifier* modifier = emitter.modifiers[i];
		nlohmann::json modData = json["property_modifiers"][i];
		modifier->Deserialise(modData);
	}
}
