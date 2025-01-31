#pragma once

#include "Particle.h"

class ParticlePropertyModifier
{
public:

	ParticlePropertyModifier() = default;
	virtual ~ParticlePropertyModifier() = default;

	virtual void ApplyModifier(Particle& target,double deltatime) = 0;
};
