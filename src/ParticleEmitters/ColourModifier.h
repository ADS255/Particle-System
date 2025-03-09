#pragma once

#include "ParticlePropertyModifier.h"

class ColourModifier : public ParticlePropertyModifier<glm::vec4>
{
public:
	ColourModifier();
	~ColourModifier();

private:

	void ApplyModifier(Particle& target, double deltatime) override;

	void ConstantModeUI() override;
	void RandBetweenTwoConstantsUI() override;
};
