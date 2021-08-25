#include "CreditsScreen.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "AppWindow.h"
#include "SwapChain.h"
#include "Texture.h"
#include "UIManager.h"

CreditsScreen::CreditsScreen() : AUIScreen("CreditsScreen")
{
	m_logo = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\UI\\Images\\dlsu_logo.png");
}

CreditsScreen::~CreditsScreen()
{
	AUIScreen::~AUIScreen();
}

void CreditsScreen::drawUI()
{
	if (!m_is_open)
	{
		UINames uiNames;
		closeWindow(uiNames.CREDITS_SCREEN);

		delete this;
		
		return;
	}

	ImGui::Begin("Credits", &m_is_open);
	ImGui::Image((void*)m_logo->getTexture(), ImVec2(200, 200));
	ImGui::Text("\n\nAbout\n\n");
	ImGui::Text("Scene Editor v0.02\n");
	ImGui::Text("Developed by: Jacob V. Manzano\n\n");
	ImGui::Text("Acknowledgements:\n");
	ImGui::Text("PardCode Game Engine Tutorial\n");
	ImGui::Text("Sir Neil's GDENG-2 Course");
	ImGui::End();
}