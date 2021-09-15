#pragma once
#include "AUIScreen.h"

#include <imfilebrowser.h>

class MenuBarScreen : public AUIScreen
{
private:
	MenuBarScreen();
	~MenuBarScreen();

	virtual void drawUI() override;
	void onCreateCreditsScreen();
	void onCreateActionScreen();
	void onCreateColorPickerScreen();
	void onCreateConsoleScreen();
	void onCreatePlaybackScreen();
	void onCreateProfilerScreen();
	void onCreateViewportScreen();

	// Initialize as -1 for no viewport limit
	int m_viewport_limit = 3;

	bool m_is_focused = false;
	bool m_is_hovered = false;

	ImGui::FileBrowser* m_save_scene_dialog;
	ImGui::FileBrowser* m_open_scene_dialog;

	friend class UIManager;
};