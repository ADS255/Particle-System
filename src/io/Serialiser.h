#pragma once

#include <json.hpp>

#include "FileHandler.h"
#include "ParticleEmitter.h"

class Serialiser
{
public:
	Serialiser();
	~Serialiser();

	void SerialiseParticleEmitter(ParticleEmitter &emitter);
	void DeserialiseParticleEmitter(ParticleEmitter& emitter);

	/*
	std::vector<IParticlePropertyModifier*> modifiers;
	std::vector<IParticlePropertyOverLifetimeModifer*> overLifetimeModifiers;
	*/

protected:


};