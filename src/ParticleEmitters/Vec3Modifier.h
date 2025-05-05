#pragma once

#include "ParticlePropertyModifier.h"

class Vec3Modifier : public ParticlePropertyModifier<glm::vec3>
{
public:
	Vec3Modifier(std::string modiferName);
	~Vec3Modifier();

protected:

	void ConstantModeUI() override;
	void RandBetweenTwoConstantsUI() override;
	void Serialise(nlohmann::json& json) override;
	void Deserialise(nlohmann::json& json) override;
	std::vector<float> AsFloats() override;
};