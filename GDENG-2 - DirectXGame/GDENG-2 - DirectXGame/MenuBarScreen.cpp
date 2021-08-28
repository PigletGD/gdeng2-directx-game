#include "MenuBarScreen.h"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "CreditsScreen.h"
#include "ColorPickerScreen.h"
#include "ViewportScreen.h"
#include "UIManager.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "AppWindow.h"
#include "GameObjectManager.h"

MenuBarScreen::MenuBarScreen() : AUIScreen("Menu Bar")
{
}

MenuBarScreen::~MenuBarScreen()
{
}

void MenuBarScreen::drawUI()
{
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("MainWindow", nullptr, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

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
		if (ImGui::BeginMenu("Game Objects"))
		{
			if (ImGui::BeginMenu("Cubes")) {
				if (ImGui::MenuItem("Cube")) { GameObjectManager::getInstance()->createObject(GameObjectManager::PrimitiveType::CUBE); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Textured Cube")) { GameObjectManager::getInstance()->createObject(GameObjectManager::PrimitiveType::TEXTURED_CUBE); ImGui::SetWindowFocus(nullptr); }
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Plane")) { GameObjectManager::getInstance()->createObject(GameObjectManager::PrimitiveType::PLANE); ImGui::SetWindowFocus(nullptr); }
			if (ImGui::BeginMenu("Meshes")) {
				if (ImGui::MenuItem("Teapot")) { GameObjectManager::getInstance()->createObject(L"Assets\\Meshes\\teapot.obj", L"Assets\\Textures\\brick.png"); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Bunny")) { GameObjectManager::getInstance()->createObject(L"Assets\\Meshes\\bunny.obj", L"Assets\\Textures\\brick.png"); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Armadillo")) { GameObjectManager::getInstance()->createObject(L"Assets\\Meshes\\armadillo.obj", L"Assets\\Textures\\brick.png"); ImGui::SetWindowFocus(nullptr); }
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			RenderSystem* rs = GraphicsEngine::get()->getRenderSystem();
			CameraSystem* camera_system = GraphicsEngine::get()->getCameraSystem();
			Camera* camera = GraphicsEngine::get()->getCameraSystem()->getCurrentCamera();

			if (ImGui::BeginMenu("Viewmode"))
			{
				if (ImGui::MenuItem("Normal")) { camera->setToNormalViewMode(); ImGui::SetWindowFocus(nullptr); }
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

	ImGui::End();
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
