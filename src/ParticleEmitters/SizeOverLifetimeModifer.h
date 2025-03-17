#pragma once

#include "ParticlePropertyOverLifetimeModifer.h"

class SizeOverLifetimeModifier : public ParticlePropertyOverLifetimeModifer<float>
{
public:
	SizeOverLifetimeModifier();
	~SizeOverLifetimeModifier();

protected:

	void ApplyModifier(Particle& target, double deltatime) override;

	void ModifierUI() override;

};