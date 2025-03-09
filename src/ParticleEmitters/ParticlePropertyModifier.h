#pragma once

#include <imgui.h>
#include <glm/glm.hpp>
#include <random>

#include "IParticlePropertyModifier.h"

enum class PropertyMode
{
	Constant,
	RandomBetweenTwoConstants
};

template<typename T>
class ParticlePropertyModifier : public IParticlePropertyModifier
{
public:

	ParticlePropertyModifier(std::string modifierName) : mode(PropertyMode::Constant), constantValue(T()), minValue(T()), maxValue(T()),modifierName(modifierName){};
	~ParticlePropertyModifier() {};

	void ApplyModifier(Particle& target, double deltatime) override = 0;

	void Editor() override {

		ImGui::Separator();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 10));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		ImGui::Text(modifierName.c_str());
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		const char* modes[] = { "Constant","Random Between Two Constants" };
		int currentMode = static_cast<int>(mode);

		ImGui::Text("Mode");
		if (ImGui::Combo(("##mode_" + modifierName).c_str(), &currentMode, modes, IM_ARRAYSIZE(modes)))
		{
			mode = static_cast<PropertyMode>(currentMode);
		}

		switch (mode)
		{
		case PropertyMode::Constant:
			ConstantModeUI();
			break;

		case PropertyMode::RandomBetweenTwoConstants:
			RandBetweenTwoConstantsUI();
			break;
		}

	}

protected:
	std::string modifierName;

	PropertyMode mode;
	T constantValue;
	T minValue;
	T maxValue;

	virtual void ConstantModeUI() = 0;
	virtual void RandBetweenTwoConstantsUI() = 0;

	/*
	virtual void ConstantModeUI() {
		ImGui::LabelText(("##ConstantValue_" + modifierName).c_str(), ("Value " + modifierName).c_str());
		ImGui::InputFloat3(("##ConstantInput_" + modifierName).c_str(), &constantValue[0]);
	}

	virtual void RandBetweenTwoConstantsUI() {
		ImGui::LabelText(("##MinValue_" + modifierName).c_str(), ("Min " + modifierName).c_str());
		ImGui::InputFloat3(("##MinInput_" + modifierName).c_str(), &minValue[0]);

		ImGui::LabelText(("##MaxValue_" + modifierName).c_str(), ("Max " + modifierName).c_str());
		ImGui::InputFloat3(("##MaxInput_" + modifierName).c_str(), &maxValue[0]);
	}
	*/

	T GetValue() const
	{
		switch (mode)
		{
		case PropertyMode::Constant:
			return constantValue;

		case PropertyMode::RandomBetweenTwoConstants:
			return RandomBetween(minValue, maxValue);

		default:
			return constantValue;
		}
	}

	float RandomBetween(float min, float max) const
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(min, max);

		return dist(gen);
	}

	glm::vec3 RandomBetween(const glm::vec3& min, const glm::vec3& max) const
	{
		return glm::vec3(
			RandomBetween(min[0], max[0]),
			RandomBetween(min[1], max[1]),
			RandomBetween(min[2], max[2])
		);
	}

	glm::vec4 RandomBetween(const glm::vec4& min, const glm::vec4& max) const
	{
		return glm::vec4(
			RandomBetween(min[0], max[0]),
			RandomBetween(min[1], max[1]),
			RandomBetween(min[2], max[2]),
			RandomBetween(min[3], max[3])
		);
	}
};
