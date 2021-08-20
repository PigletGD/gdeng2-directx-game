#include "MenuBarScreen.h"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "CreditsScreen.h"
#include "ColorPickerScreen.h"
#include "ViewportScreen.h"
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
			if (ImGui::MenuItem("Viewport")) { onCreateViewportScreen(); }
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

void MenuBarScreen::onCreateViewportScreen()
{
	UIManager* uiManager = UIManager::getInstance();
	UINames uiNames;

	String viewportName = uiNames.VIEWPORT_SCREEN + "_0";
	int i = 0;

	while (uiManager->uiTable[viewportName] != nullptr) {
		i++;

		viewportName = uiNames.VIEWPORT_SCREEN + "_" + std::to_string(i);
	}

	/*
	if (uiManager->uiTable[uiNames.VIEWPORT_SCREEN] == nullptr) {
		ViewportScreen* viewportScreen = new ViewportScreen();
		uiManager->uiTable[uiNames.VIEWPORT_SCREEN] = viewportScreen;
		uiManager->uiList.push_back(viewportScreen);

		std::cout << "Created Viewport Screen" << std::endl;
	}
	else std::cout << "Viewport Screen Already Created" << std::endl;
	*/
	
	String name = "Viewport" + std::to_string(i);

	ViewportScreen* viewportScreen = new ViewportScreen(name);
	uiManager->uiTable[viewportName] = viewportScreen;
	uiManager->uiList.push_back(viewportScreen);

	std::cout << "Created Viewport Screen" << std::endl;

}
