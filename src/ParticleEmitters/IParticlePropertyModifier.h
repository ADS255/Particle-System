#pragma once

#include <json.hpp>

#include "Particle.h"

class IParticlePropertyModifier
{
public:
	virtual ~IParticlePropertyModifier() {};

	virtual void ApplyModifier(Particle& target, double deltatime) = 0;
	virtual void Editor() = 0;

	virtual void Serialise(nlohmann::json& json) = 0;
	virtual void Deserialise(nlohmann::json& json) = 0;
	
	virtual std::vector<float> AsFloats() = 0;
};