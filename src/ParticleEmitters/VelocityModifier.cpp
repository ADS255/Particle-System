#include "VelocityModifier.h"

VelocityModifier::VelocityModifier()
	: Vec3Modifier("Velocity")
{

}

VelocityModifier::~VelocityModifier()
{
}

void VelocityModifier::ApplyModifier(Particle& target, double deltatime)
{
	target.velocity = GetValue();
}
