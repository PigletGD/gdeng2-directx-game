#include "ColorPickerScreen.h"

#include "UIManager.h"

ColorPickerScreen::ColorPickerScreen() :
	AUIScreen("Color Picker Screen")
{
}

ColorPickerScreen::~ColorPickerScreen()
{
}

void ColorPickerScreen::drawUI()
{
	if (!m_is_open)
	{
		UINames uiNames;
		closeWindow(uiNames.COLOR_PICKER_SCREEN);

		delete this;

		return;
	}

	ImGui::Begin("Color Picker Screen", &m_is_open);	
	ImGui::ColorPicker3("Color", (float*)&m_color, 
		ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
	ImGui::End();
}
