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

void Vec3Modifier::Serialise(nlohmann::json& json)
{
	json["property_modifiers"].push_back({
	{"mode", mode},
	{"constant_value", {constantValue[0],constantValue[1],constantValue[2]}},
	{"min_value", {minValue[0],minValue[1],minValue[2]}},
	{"max_value", {maxValue[0],maxValue[1],maxValue[2]}}
		});
}

void Vec3Modifier::Deserialise(nlohmann::json& json)
{
	mode = static_cast<PropertyMode>(json["mode"].get<int>());
	constantValue = glm::vec3(json["constant_value"][0].get<float>(), json["constant_value"][1].get<float>(), json["constant_value"][2].get<float>());
	minValue = glm::vec3(json["min_value"][0].get<float>(), json["min_value"][1].get<float>(), json["min_value"][2].get<float>());
	maxValue = glm::vec3(json["max_value"][0].get<float>(), json["max_value"][1].get<float>(), json["max_value"][2].get<float>());
}
