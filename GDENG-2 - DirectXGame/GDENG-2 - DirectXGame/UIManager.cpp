#include "UIManager.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "CreditsScreen.h"
#include "MenuBarScreen.h"
#include "AppWindow.h"
#include "SwapChain.h"
#include "ObjectHeirarchy.h"

UIManager* UIManager::sharedInstance = NULL;

UIManager::UIManager(HWND windowHandle)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplDX11_Init(GraphicsEngine::get()->getRenderSystem()->getDevice(), GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->getContext());

	UINames uiNames;
	MenuBarScreen* menuBarScreen = new MenuBarScreen();
	uiTable[uiNames.MENU_BAR_SCREEN] = menuBarScreen;
	uiList.push_back(menuBarScreen);

	ObjectHeirarchy* objectHeirarchyScreen = new ObjectHeirarchy(uiNames.OBJECT_HEIRARCHY_SCREEN);
	uiTable[uiNames.OBJECT_HEIRARCHY_SCREEN] = objectHeirarchyScreen;
	uiList.push_back(objectHeirarchyScreen);
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
