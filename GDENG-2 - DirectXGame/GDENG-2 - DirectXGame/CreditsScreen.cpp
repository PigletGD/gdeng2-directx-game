#include "CreditsScreen.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

CreditsScreen::CreditsScreen() : AUIScreen("CreditsScreen")
{
}

CreditsScreen::~CreditsScreen()
{
	AUIScreen::~AUIScreen();
}

void CreditsScreen::drawUI()
{
	ImGui::Begin("Credits Screen");
	ImGui::Text("About\n\n");
	ImGui::Text("Scene Editor v0.01\n");
	ImGui::Text("Developed by: Jacob V. Manzano\n\n");
	ImGui::Text("Acknowledgements:\n");
	ImGui::Text("PardCode Game Engine Tutorial\n");
	ImGui::Text("Sir Neil's GDENG-2 Course");
	ImGui::End();
}
