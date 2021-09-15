#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Window.h"
#include "AUIScreen.h"

class UINames {
public:
	const String MENU_BAR_SCREEN = "MENU_BAR_SCREEN";
	const String ACTION_SCREEN = "ACTION_SCREEN";
	const String CREDITS_SCREEN = "CREDITS_SCREEN";
	const String COLOR_PICKER_SCREEN = "COLOR_PICKER_SCREEN";
	const String CONSOLE_SCREEN = "CONSOLE_SCREEN";
	const String HIERARCHY_SCREEN = "HIERARCHY_SCREEN";
	const String INSPECTOR_SCREEN = "INSPECTOR_SCREEN";
	const String PLAYBACK_SCREEN = "PLAYBACK_SCREEN";
	const String PROFILER_SCREEN = "PROFILER_SCREEN";
	const String VIEWPORT_SCREEN = "VIEWPORT_SCREEN";
};

class UIManager
{
public:
	typedef std::vector<AUIScreen*> UIList;
	typedef std::unordered_map<String, AUIScreen*> UITable;

	static UIManager* getInstance();
	static void initialize(HWND windowHandle);
	static void destroy();

	void drawAllUI();

	static const int WINDOW_WIDTH = 1024;
	static const int WINDOW_HEIGHT = 768;

private:
	UIManager(HWND windowHandle);
	~UIManager();
	UIManager(UIManager const&) {};
	UIManager& operator=(UIManager const&) {};
	static UIManager* sharedInstance;

	UIList uiList;
	UITable uiTable;

	friend class MenuBarScreen;
	friend class AUIScreen;
};