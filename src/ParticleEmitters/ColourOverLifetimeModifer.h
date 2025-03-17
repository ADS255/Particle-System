#pragma once

#include "ParticlePropertyOverLifetimeModifer.h"

class ColourOverLifetimeModifer: public ParticlePropertyOverLifetimeModifer<glm::vec4>
{
public:
	ColourOverLifetimeModifer();
	~ColourOverLifetimeModifer();

protected:

	void ApplyModifier(Particle& target, double deltatime) override;

	void ModifierUI() override;

};

