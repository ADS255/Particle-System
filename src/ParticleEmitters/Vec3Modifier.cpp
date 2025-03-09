#include "Vec3Modifier.h"

Vec3Modifier::Vec3Modifier(std::string modiferName)
	: ParticlePropertyModifier<glm::vec3>(modiferName)
{
}

Vec3Modifier::~Vec3Modifier()
{
}

void Vec3Modifier::ConstantModeUI()
{
	ImGui::LabelText(("##ConstantValue_" + modifierName).c_str(), ("Value " + modifierName).c_str());
	ImGui::InputFloat3(("##ConstantInput_" + modifierName).c_str(), &constantValue[0]);
}

void Vec3Modifier::RandBetweenTwoConstantsUI()
{
	ImGui::LabelText(("##MinValue_" + modifierName).c_str(), ("Min " + modifierName).c_str());
	ImGui::InputFloat3(("##MinInput_" + modifierName).c_str(), &minValue[0]);

	ImGui::LabelText(("##MaxValue_" + modifierName).c_str(), ("Max " + modifierName).c_str());
	ImGui::InputFloat3(("##MaxInput_" + modifierName).c_str(), &maxValue[0]);
}
