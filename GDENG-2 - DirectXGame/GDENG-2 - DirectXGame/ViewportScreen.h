#pragma once
#include "AUIScreen.h"

#include <d3d11.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "RenderTexture.h"
#include "Camera.h"

class ViewportScreen : public AUIScreen
{
private:
	ViewportScreen(String name, int index);
	~ViewportScreen();

	virtual void drawUI() override;

	String m_name;
	String m_window_name;

	ID3D11RasterizerState* m_rs;
	RenderTexture* m_rt;
	Camera* m_camera;
	ImVec2 m_old_size;

	bool m_is_focused = false;
	bool m_is_hovered = false;

	friend class MenuBarScreen;
};