#include "FloatModifier.h"

FloatModifier::FloatModifier(std::string modiferName)
	: ParticlePropertyModifier<float>(modiferName)
{
}

FloatModifier::~FloatModifier()
{
}

void FloatModifier::ConstantModeUI()
{
	ImGui::LabelText(("##ConstantValue_" + modifierName).c_str(), ("Value " + modifierName).c_str());
	ImGui::InputFloat(("##ConstantInput_" + modifierName).c_str(), &constantValue);
}

void FloatModifier::RandBetweenTwoConstantsUI()
{
	ImGui::LabelText(("##MinValue_" + modifierName).c_str(), ("Min " + modifierName).c_str());
	ImGui::InputFloat(("##MinInput_" + modifierName).c_str(), &minValue);

	ImGui::LabelText(("##MaxValue_" + modifierName).c_str(), ("Max " + modifierName).c_str());
	ImGui::InputFloat(("##MaxInput_" + modifierName).c_str(), &maxValue);
}

void FloatModifier::Serialise(nlohmann::json& json)
{
	json["property_modifiers"].push_back({
		{"mode", mode},
		{"constant_value", constantValue},
		{"min_value", minValue},
		{"max_value", maxValue}
		});
}

void FloatModifier::Deserialise(nlohmann::json& json)
{
	mode = static_cast<PropertyMode>(json["mode"].get<int>());
	constantValue = json["constant_value"].get<float>();
	minValue = json["min_value"].get<float>();
	maxValue = json["max_value"].get<float>();
}
