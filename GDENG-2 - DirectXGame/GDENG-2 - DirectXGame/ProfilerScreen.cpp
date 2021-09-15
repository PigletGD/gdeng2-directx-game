#include "ProfilerScreen.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "UIManager.h"

ProfilerScreen::ProfilerScreen() :AUIScreen("ProfilerScreen")
{

}

ProfilerScreen::~ProfilerScreen()
{
	AUIScreen::~AUIScreen();
}

void ProfilerScreen::drawUI()
{
	if (!m_is_open)
	{
		UINames uiNames;
		closeWindow(uiNames.PROFILER_SCREEN);

		delete this;

		return;
	}

	ImGui::Begin("Profiler", &m_is_open);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}