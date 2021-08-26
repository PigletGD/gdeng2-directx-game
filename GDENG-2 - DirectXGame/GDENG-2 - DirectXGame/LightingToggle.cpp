#include "LightingToggle.h"

#include "UIManager.h"

LightingToggle::LightingToggle() :
	AUIScreen("Lighting Toggle")
{
}

LightingToggle::~LightingToggle()
{
}

void LightingToggle::drawUI()
{
	static bool isLit = false;
	if (!m_is_open)
	{
		UINames uiNames;
		closeWindow(uiNames.LIGHTING_TOGGLE);

		delete this;

		return;
	}

	//std::cout << cc.isLit << std::endl;

	ImGui::Begin("Lighting Toggle", &m_is_open);
	ImGui::Checkbox("Toggle", &isLit);

	if (isLit) {
		this->cc.isLit = 1;
	}

	else if (!isLit) {
		this->cc.isLit = 2;
	}
	std::cout << this->cc.isLit << std::endl;
	ImGui::End();
}