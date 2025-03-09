#pragma once

#include "FloatModifier.h"

class SizeModifier : public FloatModifier
{
public:
	SizeModifier();
	~SizeModifier();

	void ApplyModifier(Particle& target, double deltatime) override;

protected:

private:
};
