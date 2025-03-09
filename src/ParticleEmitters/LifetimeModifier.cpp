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
	target.lifetime = GetValue();
}