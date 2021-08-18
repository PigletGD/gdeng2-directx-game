#pragma once
#include "AUIScreen.h"

#include "GraphicsEngine.h"

class CreditsScreen : public AUIScreen
{
private:
	CreditsScreen();
	~CreditsScreen();

	virtual void drawUI() override;

	TexturePtr m_logo;

	friend class MenuBarScreen;
	friend class UIManager;
};