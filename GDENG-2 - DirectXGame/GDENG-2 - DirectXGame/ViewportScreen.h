#pragma once
#include "AUIScreen.h"

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

	friend class MenuBarScreen;
};

