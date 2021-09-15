#include "ConsoleScreen.h"

#include <sstream>

#include "UIManager.h"
#include "Debug.h"

void ConsoleScreen::appendText(String text)
{
	std::stringstream buffer;
	buffer << m_line_count << " " << text;

	m_text_log->appendf(buffer.str().c_str());
	m_line_count++;
}

ConsoleScreen::ConsoleScreen() : AUIScreen("ConsoleScreen")
{
	Debug::assignConsole(this);

	m_text_log = new ImGuiTextBuffer();
}

ConsoleScreen::~ConsoleScreen()
{
	Debug::assignConsole(NULL);

	delete m_text_log;
}

void ConsoleScreen::drawUI()
{
	if (!m_is_open)
	{
		UINames uiNames;
		closeWindow(uiNames.CONSOLE_SCREEN);

		delete this;

		return;
	}

	ImGui::Begin("Console", &m_is_open, ImGuiWindowFlags_NoResize);

	ImGui::SetWindowSize(ImVec2(600, 300));
	
	if (ImGui::Button("Clear")) { m_text_log->clear(); m_line_count = 0; }
	
	ImGui::TextUnformatted(m_text_log->begin(), m_text_log->end());
	
	ImGui::End();
}