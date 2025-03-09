#pragma once

#include "ParticlePropertyModifier.h"

class FloatModifier : public ParticlePropertyModifier<float>
{
public:
	FloatModifier(std::string modiferName);
	~FloatModifier();

protected:
	
	void ConstantModeUI() override;
	void RandBetweenTwoConstantsUI() override;
};
