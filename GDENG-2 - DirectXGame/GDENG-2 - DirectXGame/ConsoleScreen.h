#pragma once

#include "AUIScreen.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

class ConsoleScreen : public AUIScreen
{
public:
	void appendText(String text);
private:
	ConsoleScreen();
	~ConsoleScreen();

	virtual void drawUI() override;

	ImGuiTextBuffer* m_text_log;
	int m_line_count = 0;

	friend class MenuBarScreen;
	friend class UIManager;
};