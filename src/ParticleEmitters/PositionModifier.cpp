#include "PositionModifier.h"

PositionModifier::PositionModifier()
	: Vec3Modifier("Position")
{

}

PositionModifier::~PositionModifier()
{
}

void PositionModifier::ApplyModifier(Particle& target, double deltatime)
{
	target.position = GetValue();
}
