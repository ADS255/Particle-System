#pragma once

#include "Particle.h"

class IParticlePropertyModifier
{
public:
	virtual ~IParticlePropertyModifier() {};
	virtual void ApplyModifier(Particle& target, double deltatime) = 0;
	virtual void Editor() = 0;
};