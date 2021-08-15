#pragma once
#include "AUIScreen.h"

class CreditsScreen : public AUIScreen
{
	CreditsScreen();
	~CreditsScreen();

	virtual void drawUI() override;
	friend class UIManager;
};

