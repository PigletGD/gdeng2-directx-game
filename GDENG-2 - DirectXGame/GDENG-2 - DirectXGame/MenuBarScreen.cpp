#include "MenuBarScreen.h"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "CreditsScreen.h"
#include "ColorPickerScreen.h"
#include "UIManager.h"

MenuBarScreen::MenuBarScreen() : AUIScreen("Menu Bar")
{
}

MenuBarScreen::~MenuBarScreen()
{
}

void MenuBarScreen::drawUI()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("About"))
		{
			if (ImGui::MenuItem("Credits")) { onCreateCreditsScreen(); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Color Picker")) { onCreateColorPickerScreen(); }
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void MenuBarScreen::onCreateCreditsScreen()
{
	UIManager* uiManager = UIManager::getInstance();
	UINames uiNames;

	if (uiManager->uiTable[uiNames.CREDITS_SCREEN] == nullptr) {
		CreditsScreen* creditsScreen = new CreditsScreen();
		uiManager->uiTable[uiNames.CREDITS_SCREEN] = creditsScreen;
		uiManager->uiList.push_back(creditsScreen);
		
		std::cout << "Created Credits Screen" << std::endl;
	}
	else std::cout << "Credits Screen Already Created" << std::endl;
}

void MenuBarScreen::onCreateColorPickerScreen()
{
	UIManager* uiManager = UIManager::getInstance();
	UINames uiNames;

	if (uiManager->uiTable[uiNames.COLOR_PICKER_SCREEN] == nullptr) {
		ColorPickerScreen* colorPickerScreen = new ColorPickerScreen();
		uiManager->uiTable[uiNames.COLOR_PICKER_SCREEN] = colorPickerScreen;
		uiManager->uiList.push_back(colorPickerScreen);

		std::cout << "Created Color Picker Screen" << std::endl;
	}
	else std::cout << "Color Picker Screen Already Created" << std::endl;
}
