#pragma once

#include "FloatModifier.h"

class LifetimeModifier : public FloatModifier
{
public:
	LifetimeModifier();
	~LifetimeModifier();

	void ApplyModifier(Particle& target, double deltatime) override;

protected:

private:
};
