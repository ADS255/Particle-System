#include "LifetimeModifier.h"

LifetimeModifier::LifetimeModifier()
	: FloatModifier("Lifetime")
{
}

LifetimeModifier::~LifetimeModifier()
{
}

void LifetimeModifier::ApplyModifier(Particle& target, double deltatime)
{
	float value = GetValue();
	target.lifetime = value;
	target.maxLifetime = value;
}