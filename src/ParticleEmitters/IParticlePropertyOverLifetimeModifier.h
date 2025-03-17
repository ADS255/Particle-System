#pragma once

#include "Particle.h"

class IParticlePropertyOverLifetimeModifer
{
public:
	virtual ~IParticlePropertyOverLifetimeModifer() {};
	virtual void ApplyModifier(Particle& target, double deltatime) = 0;
	virtual void Editor() = 0;
};