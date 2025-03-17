#include "ColourOverLifetimeModifer.h"

ColourOverLifetimeModifer::ColourOverLifetimeModifer()
	:ParticlePropertyOverLifetimeModifer<glm::vec4>("Colour Over Lifetime")
{
	startValue = glm::vec4(1.0f);
	endValue = glm::vec4(1.0f);
}

ColourOverLifetimeModifer::~ColourOverLifetimeModifer()
{
}

void ColourOverLifetimeModifer::ApplyModifier(Particle& target, double deltatime)
{
	target.colour = Interpolate(target, startValue, endValue);

	//target.colour = glm::vec4(1.0f,0.0f,0.0f,1.0f);
}

void ColourOverLifetimeModifer::ModifierUI()
{
	ImGui::LabelText(("##StartValue_" + modifierName).c_str(),"Start Value");
	ImGui::ColorEdit4(("##StartInput_" + modifierName).c_str(), &startValue[0]);

	ImGui::LabelText(("##EndValue_" + modifierName).c_str(),"End Value");
	ImGui::ColorEdit4(("##EndInput_" + modifierName).c_str(), &endValue[0]);
}
