#include "SizeOverLifetimeModifer.h"

SizeOverLifetimeModifier::SizeOverLifetimeModifier()
	:ParticlePropertyOverLifetimeModifer<float>("Size Over Lifetime")
{
	startValue = 0.0f;
	endValue = 1.0f;
}

SizeOverLifetimeModifier::~SizeOverLifetimeModifier()
{
}

void SizeOverLifetimeModifier::ApplyModifier(Particle& target, double deltatime)
{
	target.size = Interpolate(target,startValue,endValue);
}

void SizeOverLifetimeModifier::ModifierUI()
{
	ImGui::LabelText(("##StartValue_" + modifierName).c_str(), "Start Value");
	ImGui::InputFloat(("##StartInput_" + modifierName).c_str(), &startValue);

	ImGui::LabelText(("##EndValue_" + modifierName).c_str(), "End Value");
	ImGui::InputFloat(("##EndInput_" + modifierName).c_str(), &endValue);
}
