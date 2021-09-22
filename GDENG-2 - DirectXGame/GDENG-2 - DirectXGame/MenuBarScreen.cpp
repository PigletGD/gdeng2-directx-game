#include "MenuBarScreen.h"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "CreditsScreen.h"
#include "ActionScreen.h"
#include "ColorPickerScreen.h"
#include "PlaybackScreen.h"
#include "ViewportScreen.h"
#include "UIManager.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "AppWindow.h"
#include "GameObjectManager.h"
#include "SceneReader.h"
#include "SceneWriter.h"
#include "ConsoleScreen.h"
#include "ProfilerScreen.h"

MenuBarScreen::MenuBarScreen() : AUIScreen("Menu Bar")
{
	m_open_scene_dialog = new ImGui::FileBrowser();
	m_open_scene_dialog->SetTitle("Open Scene");
	m_open_scene_dialog->SetTypeFilters({ ".level" });

	m_save_scene_dialog = new ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
	m_save_scene_dialog->SetTitle("Save Scene");
	m_save_scene_dialog->SetTypeFilters({ ".level" });
}

MenuBarScreen::~MenuBarScreen()
{
	delete m_open_scene_dialog;
	delete m_save_scene_dialog;
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
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { m_open_scene_dialog->Open(); }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { m_save_scene_dialog->Open(); }
			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) { m_save_scene_dialog->Open(); }
			if (ImGui::MenuItem("Exit Editor", "Ctrl+W")) {/*Do something */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
			if (ImGui::MenuItem("Credits")) { onCreateCreditsScreen(); ImGui::SetWindowFocus(nullptr); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Action")) { onCreateActionScreen(); ImGui::SetWindowFocus(nullptr); }
			if (ImGui::MenuItem("Color Picker")) { onCreateColorPickerScreen(); ImGui::SetWindowFocus(nullptr); }
			if (ImGui::MenuItem("Console")) { onCreateConsoleScreen(); ImGui::SetWindowFocus(nullptr); }
			if (ImGui::MenuItem("Playback")) { onCreatePlaybackScreen(); ImGui::SetWindowFocus(nullptr); }
			if (ImGui::MenuItem("Profiler")) { onCreateProfilerScreen(); ImGui::SetWindowFocus(nullptr); }
			if (ImGui::MenuItem("Viewport")) { onCreateViewportScreen(); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Game Objects"))
		{
			if (ImGui::BeginMenu("Cubes")) {
				if (ImGui::MenuItem("Cube")) { GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::CUBE); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Physics Cube")) { GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::PHYSICS_CUBE); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Textured Cube")) { GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::TEXTURED_CUBE); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Physics Cube Batch")) { GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::PHYSICS_CUBE_BATCH); ImGui::SetWindowFocus(nullptr); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Planes")) {
				if (ImGui::MenuItem("Plane")) { GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::PLANE); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Physics Plane")) { GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::PHYSICS_PLANE); ImGui::SetWindowFocus(nullptr); }
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Sphere")) { GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::SPHERE); ImGui::SetWindowFocus(nullptr); }
			if (ImGui::MenuItem("Cylinder")) { GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::CYLINDER); ImGui::SetWindowFocus(nullptr); }
			if (ImGui::BeginMenu("Meshes")) {
				if (ImGui::MenuItem("Teapot")) { GameObjectManager::getInstance()->createObject(L"Assets\\Meshes\\teapot.obj", L"Assets\\Textures\\brick.png"); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Bunny")) { GameObjectManager::getInstance()->createObject(L"Assets\\Meshes\\bunny.obj", L""); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Armadillo")) { GameObjectManager::getInstance()->createObject(L"Assets\\Meshes\\armadillo.obj", L""); ImGui::SetWindowFocus(nullptr); }
				if (ImGui::MenuItem("Lucy")) { GameObjectManager::getInstance()->createObject(L"Assets\\Meshes\\statue.obj", L""); ImGui::SetWindowFocus(nullptr); }
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

	m_open_scene_dialog->Display();
	m_save_scene_dialog->Display();

	if (m_save_scene_dialog->HasSelected())
	{
		SceneWriter writer = SceneWriter(m_save_scene_dialog->GetSelected().string());
		writer.writeToFile();

		m_save_scene_dialog->ClearSelected();
		m_save_scene_dialog->Close();
	}

	else if (m_open_scene_dialog->HasSelected()) {
		SceneReader reader = SceneReader(m_open_scene_dialog->GetSelected().string());
		reader.readFromFile();

		m_open_scene_dialog->ClearSelected();
		m_open_scene_dialog->Close();
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

void MenuBarScreen::onCreateActionScreen()
{
	UIManager* uiManager = UIManager::getInstance();
	UINames uiNames;

	if (uiManager->uiTable[uiNames.ACTION_SCREEN] == nullptr) {
		ActionScreen* actionScreen = new ActionScreen();
		uiManager->uiTable[uiNames.ACTION_SCREEN] = actionScreen;
		uiManager->uiList.push_back(actionScreen);

		std::cout << "Created Action Screen" << std::endl;
	}
	else std::cout << "Action Screen Already Created" << std::endl;
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

void MenuBarScreen::onCreateConsoleScreen()
{
	UIManager* uiManager = UIManager::getInstance();
	UINames uiNames;

	if (uiManager->uiTable[uiNames.CONSOLE_SCREEN] == nullptr) {
		ConsoleScreen* consoleScreen = new ConsoleScreen();
		uiManager->uiTable[uiNames.CONSOLE_SCREEN] = consoleScreen;
		uiManager->uiList.push_back(consoleScreen);

		std::cout << "Created Console Screen" << std::endl;
	}
	else std::cout << "Console Screen Already Created" << std::endl;
}

void MenuBarScreen::onCreatePlaybackScreen()
{
	UIManager* uiManager = UIManager::getInstance();
	UINames uiNames;

	if (uiManager->uiTable[uiNames.PLAYBACK_SCREEN] == nullptr) {
		PlaybackScreen* playbackScreen = new PlaybackScreen();
		uiManager->uiTable[uiNames.PLAYBACK_SCREEN] = playbackScreen;
		uiManager->uiList.push_back(playbackScreen);

		std::cout << "Created Playback Screen" << std::endl;
	}
	else std::cout << "Playback Screen Already Created" << std::endl;
}

void MenuBarScreen::onCreateProfilerScreen()
{
	UIManager* uiManager = UIManager::getInstance();
	UINames uiNames;

	if (uiManager->uiTable[uiNames.PROFILER_SCREEN] == nullptr) {
		ProfilerScreen* profilerScreen = new ProfilerScreen();
		uiManager->uiTable[uiNames.PROFILER_SCREEN] = profilerScreen;
		uiManager->uiList.push_back(profilerScreen);

		std::cout << "Created Profiler Screen" << std::endl;
	}
	else std::cout << "Profiler Screen Already Created" << std::endl;
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
