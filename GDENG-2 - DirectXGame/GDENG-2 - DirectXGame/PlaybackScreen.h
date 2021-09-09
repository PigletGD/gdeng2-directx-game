#pragma once
#include "AUIScreen.h"

class PlaybackScreen : public AUIScreen
{
private:
	PlaybackScreen();
	~PlaybackScreen();

	virtual void drawUI() override;

	friend class UIManager;
	friend class MenuBarScreen;

	bool m_played = false;
	bool m_paused = false;
};