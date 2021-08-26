#include "MenuBarScreen.h"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "CreditsScreen.h"
#include "ColorPickerScreen.h"
#include "ViewportScreen.h"
#include "LightingToggle.h"
#include "UIManager.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "AppWindow.h"

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
			if (ImGui::MenuItem("Credits")) { onCreateCreditsScreen(); ImGui::SetWindowFocus(nullptr); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Color Picker")) { onCreateColorPickerScreen(); ImGui::SetWindowFocus(nullptr); }
			if (ImGui::MenuItem("Viewport")) { onCreateViewportScreen(); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			RenderSystem* rs = GraphicsEngine::get()->getRenderSystem();
			CameraSystem* camera_system = GraphicsEngine::get()->getCameraSystem();
			Camera* camera = GraphicsEngine::get()->getCameraSystem()->getCurrentCamera();

			if (ImGui::BeginMenu("Viewmode"))
			{
				if (ImGui::MenuItem("Normal")) { onCreateLightingToggle(); ImGui::SetWindowFocus(nullptr); } // onCreateLightingToggle()
				if (ImGui::MenuItem("Topdown")) { camera->setToTopDownViewMode(); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Front")) { camera->setToFrontViewMode(); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Right")) { camera->setToRighViewMode(); ImGui::SetWindowFocus(nullptr); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Projection"))
			{
				if (ImGui::MenuItem("Perspective")) { camera->setToPerspectiveMode(true); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Orthographic")) { camera->setToPerspectiveMode(false); ImGui::SetWindowFocus(nullptr); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Fill"))
			{
				if (ImGui::MenuItem("Solid")) { AppWindow::get()->m_rs = rs->m_rs_solid; ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Wireframe")) { AppWindow::get()->m_rs = rs->m_rs_wireframe; ImGui::SetWindowFocus(nullptr); }
				ImGui::EndMenu();
			}

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

	while (m_viewport_limit != i && uiManager->uiTable[viewportName] != nullptr) {
		i++;

		viewportName = uiNames.VIEWPORT_SCREEN + "_" + std::to_string(i);
	}

	if (m_viewport_limit != i) {
		ViewportScreen* viewportScreen = new ViewportScreen(viewportName, i);
		uiManager->uiTable[viewportName] = viewportScreen;
		uiManager->uiList.push_back(viewportScreen);

		std::cout << "Created Viewport Screen" << std::endl;
	}
	else std::cout << "Maximum Viewport Screens Already Reached" << std::endl;
}

void MenuBarScreen::onCreateLightingToggle()
{
	UIManager* uiManager = UIManager::getInstance();
	UINames uiNames;

	if (uiManager->uiTable[uiNames.LIGHTING_TOGGLE] == nullptr) {
		LightingToggle* lightingToggle = new LightingToggle();
		uiManager->uiTable[uiNames.LIGHTING_TOGGLE] = lightingToggle;
		uiManager->uiList.push_back(lightingToggle);

		std::cout << "Created Lighting Toggle" << std::endl;
	}
	else std::cout << "Lighting Toggle Already Created" << std::endl;
}


