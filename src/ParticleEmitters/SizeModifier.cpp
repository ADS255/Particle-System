#include "SizeModifier.h"

SizeModifier::SizeModifier()
	: FloatModifier("Size")
{
}

SizeModifier::~SizeModifier()
{
}

void SizeModifier::ApplyModifier(Particle& target, double deltatime)
{
	target.size = GetValue();
}