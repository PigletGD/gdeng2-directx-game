#pragma once
#include "AUIScreen.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "ConstantData.h"

class LightingToggle : public AUIScreen
{
private:
	LightingToggle();
	~LightingToggle();

	virtual void drawUI() override;
	constant cc;

	friend class MenuBarScreen;
	friend class UIManager;

};

