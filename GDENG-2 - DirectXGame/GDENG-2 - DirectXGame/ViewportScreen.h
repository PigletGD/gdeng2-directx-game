#pragma once
#include "AUIScreen.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "RenderTexture.h"
#include "Camera.h"

class ViewportScreen : public AUIScreen
{
private:
	ViewportScreen();
	~ViewportScreen();

	virtual void drawUI() override;

	RenderTexture* m_rt;
	Camera* m_camera;
	ImVec2 oldSize;

	friend class MenuBarScreen;
};

