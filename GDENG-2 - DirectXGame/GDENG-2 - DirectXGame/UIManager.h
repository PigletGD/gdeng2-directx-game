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
	const String CREDITS_SCREEN = "CREDITS_SCREEN";
	const String COLOR_PICKER_SCREEN = "COLOR_PICKER_SCREEN";
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