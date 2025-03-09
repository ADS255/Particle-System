#include "ColourModifier.h"

#include <iostream>


ColourModifier::ColourModifier()
	: ParticlePropertyModifier<glm::vec4>("Colour")
{
	constantValue = glm::vec4(1.0f);
	minValue = glm::vec4(1.0f);
	maxValue = glm::vec4(1.0f);
}

ColourModifier::~ColourModifier()
{
}

void ColourModifier::ApplyModifier(Particle& target, double deltatime)
{
	target.colour = GetValue();
}

void ColourModifier::ConstantModeUI()
{
	ImGui::LabelText(("##ConstantValue_" + modifierName).c_str(), ("Value " + modifierName).c_str());
	ImGui::ColorEdit4(("##ConstantInput_" + modifierName).c_str(), &constantValue[0]);
}

void ColourModifier::RandBetweenTwoConstantsUI()
{
	ImGui::LabelText(("##MinValue_" + modifierName).c_str(), ("Min " + modifierName).c_str());
	ImGui::ColorEdit4(("##MinInput_" + modifierName).c_str(), &minValue[0]);

	ImGui::LabelText(("##MaxValue_" + modifierName).c_str(), ("Max " + modifierName).c_str());
	ImGui::ColorEdit4(("##MaxInput_" + modifierName).c_str(), &maxValue[0]);
}
