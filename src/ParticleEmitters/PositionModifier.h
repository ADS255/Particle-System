#pragma once

#include "Vec3Modifier.h"

class PositionModifier: public Vec3Modifier
{
public:
	PositionModifier();
	~PositionModifier();

private:

	void ApplyModifier(Particle& target, double deltatime) override;
};
