#include "UIManager.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "CreditsScreen.h"
#include "MenuBarScreen.h"

UIManager* UIManager::sharedInstance = NULL;

UIManager::UIManager(HWND windowHandle)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplDX11_Init(GraphicsEngine::get()->getRenderSystem()->getDevice(), GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->getContext());

	UINames uiNames;
	//CreditsScreen* creditsScreen = new CreditsScreen();
	//uiTable[uiNames.CREDITS_SCREEN] = creditsScreen;
	//uiList.push_back(creditsScreen);

	MenuBarScreen* menuBarScreen = new MenuBarScreen();
	uiTable[uiNames.MENU_BAR_SCREEN] = menuBarScreen;
	uiList.push_back(menuBarScreen);
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

	for (int i = 0; i < uiList.size(); i++)
		uiList[i]->drawUI();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
