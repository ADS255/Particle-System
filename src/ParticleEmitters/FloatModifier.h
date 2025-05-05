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
	void Serialise(nlohmann::json& json) override;
	void Deserialise(nlohmann::json& json) override;
	std::vector<float> AsFloats() override;
};
