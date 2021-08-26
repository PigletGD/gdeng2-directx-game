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
	void onCreateLightingToggle();

	// Initialize as -1 for no viewport limit
	int m_viewport_limit = 3;

	friend class UIManager;
};