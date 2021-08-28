#include "UIManager.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "AppWindow.h"
#include "SwapChain.h"

#include "CreditsScreen.h"
#include "MenuBarScreen.h"
#include "HierarchyScreen.h"

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
	//MenuBarScreen* menu_bar_screen = new MenuBarScreen();
	//uiTable[ui_names.MENU_BAR_SCREEN] = menu_bar_screen;
	//uiList.push_back(menu_bar_screen);

	CreditsScreen* cs = new CreditsScreen();
	uiTable[ui_names.CREDITS_SCREEN] = cs;
	uiList.push_back(cs);

	/*HierarchyScreen* hierarchy_screen = new HierarchyScreen();
	uiTable[ui_names.HIERARCHY_SCREEN] = hierarchy_screen;
	uiList.push_back(hierarchy_screen);*/
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

	for (int i = uiList.size() - 1; i >= 0; i--) {
		uiList[i]->drawUI();
	}

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setRenderTarget(AppWindow::get()->m_swap_chain);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
