#pragma once
#include "AUIScreen.h"

class MenuBarScreen : public AUIScreen
{
private:
	MenuBarScreen();
	~MenuBarScreen();

	virtual void drawUI() override;
	void onCreateCreditsScreen();
	void onCreateColorPickerScreen();
	void onCreateViewportScreen();

	// Initialize as -1 for no viewport limit
	int m_viewport_limit = 3;

	bool m_is_focused = false;
	bool m_is_hovered = false;

	friend class UIManager;
};