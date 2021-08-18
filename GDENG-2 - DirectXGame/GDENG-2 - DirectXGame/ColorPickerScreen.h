#pragma once
#include "AUIScreen.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

class ColorPickerScreen : public AUIScreen
{
private:
	ColorPickerScreen();
	~ColorPickerScreen();

	virtual void drawUI() override;

	ImColor m_color;

	friend class MenuBarScreen;
};