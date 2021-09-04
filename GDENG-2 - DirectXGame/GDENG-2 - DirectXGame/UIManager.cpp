#include "UIManager.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "AppWindow.h"
#include "SwapChain.h"

#include "MenuBarScreen.h"
#include "HierarchyScreen.h"
#include "InspectorScreen.h"
#include "CameraSystem.h"

UIManager* UIManager::sharedInstance = NULL;

UIManager::UIManager(HWND windowHandle)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplDX11_Init(GraphicsEngine::get()->getRenderSystem()->getDevice(), GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->getContext());

	UINames ui_names;
	MenuBarScreen* menu_bar_screen = new MenuBarScreen();
	uiTable[ui_names.MENU_BAR_SCREEN] = menu_bar_screen;
	uiList.push_back(menu_bar_screen);

	HierarchyScreen* hierarchy_screen = new HierarchyScreen();
	uiTable[ui_names.HIERARCHY_SCREEN] = hierarchy_screen;
	uiList.push_back(hierarchy_screen);

	InspectorScreen* inspector_screen = new InspectorScreen();
	uiTable[ui_names.INSPECTOR_SCREEN] = inspector_screen;
	uiList.push_back(inspector_screen);

	ImGui::SetWindowFocus(nullptr);
}

UIManager::~UIManager()
{
}

UIManager* UIManager::getInstance()
{
	return sharedInstance;
}

void UIManager::initialize(HWND windowHandle)
{
	sharedInstance = new UIManager(windowHandle);
}

void UIManager::destroy()
{
	ImGui::DestroyContext();
	ImGui_ImplDX11_Shutdown();

	delete sharedInstance;
}

void UIManager::drawAllUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (int i = 0; i < uiList.size(); i++) {
		uiList[i]->drawUI();
	}

	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		GraphicsEngine::get()->getCameraSystem()->setImGuiWindowFocus(true);
	else GraphicsEngine::get()->getCameraSystem()->setImGuiWindowFocus(false);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setRenderTarget(AppWindow::get()->m_swap_chain);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	if ((ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)) && !(ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemHovered())) {
		ImGui::SetWindowFocus(nullptr);
	}
}
