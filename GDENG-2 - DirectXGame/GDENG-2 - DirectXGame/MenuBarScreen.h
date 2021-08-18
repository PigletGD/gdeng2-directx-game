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

	friend class UIManager;

	bool isOpen = false;
};