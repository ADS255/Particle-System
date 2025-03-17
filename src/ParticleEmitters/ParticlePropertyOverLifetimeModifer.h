#pragma once

#include <imgui.h>
#include <glm/glm.hpp>
#include <random>

#include "IParticlePropertyOverLifetimeModifier.h"

template<typename T>
class ParticlePropertyOverLifetimeModifer : public IParticlePropertyOverLifetimeModifer
{
public:

	ParticlePropertyOverLifetimeModifer(std::string modifierName) : startValue(T()), endValue(T()), modifierName(modifierName) {};
	~ParticlePropertyOverLifetimeModifer() {};

	void ApplyModifier(Particle& target, double deltatime) override = 0;

	void Editor() override {

		ImGui::Separator();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 10));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		ImGui::Text(modifierName.c_str());
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::Checkbox(("Enabled##lifetimeModifierEnabled_" + modifierName).c_str(), &enabled);

		if (enabled) {
			ModifierUI();
		}
	}

	virtual void ModifierUI() = 0;

protected:
	std::string modifierName;

	T startValue;

	T endValue;

	float Interpolate(Particle& target, float startValue, float endValue) const
	{
		float t = 1.0f - target.lifetime / target.maxLifetime;
		return startValue + t * (endValue - startValue);
	}

	glm::vec4 Interpolate(Particle& target, glm::vec4 startValue, glm::vec4 endValue) const
	{
		glm::vec4 colour = glm::vec4();

		float t = 1.0f - target.lifetime / target.maxLifetime;

		colour[0] = startValue[0] + t * (endValue[0] - startValue[0]);
		colour[1] = startValue[1] + t * (endValue[1] - startValue[1]);
		colour[2] = startValue[2] + t * (endValue[2] - startValue[2]);
		colour[3] = startValue[3] + t * (endValue[3] - startValue[3]);

		return colour;
	}
};
