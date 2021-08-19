#pragma once
#include "AUIScreen.h"

#include "RenderTexture.h"

class ViewportScreen : public AUIScreen
{
private:
	ViewportScreen();
	~ViewportScreen();

	virtual void drawUI() override;

	RenderTexture* m_rt;

	friend class MenuBarScreen;
};

