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

	json["emission_rate"] = emitter.editorProperties.particlesPerSecond;
	json["max_count"] = emitter.editorProperties.particleCount;
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

	emitter.editorProperties.particlesPerSecond = json["emission_rate"].get<unsigned int>();
	emitter.editorProperties.particleCount = json["max_count"].get<unsigned int>();

	for (size_t i = 0; i < emitter.modifiers.size(); ++i) {
		IParticlePropertyModifier* modifier = emitter.modifiers[i];
		nlohmann::json modData = json["property_modifiers"][i];
		modifier->Deserialise(modData);
	}
}
