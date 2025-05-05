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

void ColourModifier::Serialise(nlohmann::json& json)
{
	json["property_modifiers"].push_back({
		{"mode", mode},
		{"constant_value", {constantValue[0],constantValue[1],constantValue[2],constantValue[3]}},
		{"min_value", {minValue[0],minValue[1],minValue[2],minValue[3]}},
		{"max_value", {maxValue[0],maxValue[1],maxValue[2],maxValue[3]}}
	});
}

void ColourModifier::Deserialise(nlohmann::json& json)
{
	mode = static_cast<PropertyMode>(json["mode"].get<int>());
	constantValue = glm::vec4(json["constant_value"][0].get<float>(), json["constant_value"][1].get<float>(), json["constant_value"][2].get<float>(), json["constant_value"][3].get<float>());
	minValue = glm::vec4(json["min_value"][0].get<float>(), json["min_value"][1].get<float>(), json["min_value"][2].get<float>(), json["min_value"][3].get<float>());
	maxValue = glm::vec4(json["max_value"][0].get<float>(), json["max_value"][1].get<float>(), json["max_value"][2].get<float>(), json["max_value"][3].get<float>());
}

std::vector<float> ColourModifier::AsFloats()
{
	std::vector<float> properties;

	switch (mode)
	{
	case PropertyMode::Constant:
		properties.push_back(constantValue[0]);
		properties.push_back(constantValue[1]);
		properties.push_back(constantValue[2]);
		properties.push_back(constantValue[3]);

		properties.push_back(constantValue[0]);
		properties.push_back(constantValue[1]);
		properties.push_back(constantValue[2]);
		properties.push_back(constantValue[3]);
		break;
	case PropertyMode::RandomBetweenTwoConstants:
		properties.push_back(minValue[0]);
		properties.push_back(minValue[1]);
		properties.push_back(minValue[2]);
		properties.push_back(minValue[3]);

		properties.push_back(maxValue[0]);
		properties.push_back(maxValue[1]);
		properties.push_back(maxValue[2]);
		properties.push_back(maxValue[3]);
		break;
	}

	return properties;
}
