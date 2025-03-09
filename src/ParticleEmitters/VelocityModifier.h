#pragma once

#include "Vec3Modifier.h"

class VelocityModifier: public Vec3Modifier
{
public:
	VelocityModifier();
	~VelocityModifier();

private:

	void ApplyModifier(Particle& target, double deltatime) override;
};
