#pragma once

#include <windows.h>

class Window
{
public:
	Window();
	~Window();

	bool init();
	bool broadcast();
	bool release();
	bool isRun();

	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy();

private:
	HWND m_HWND;
	bool m_isRun;
};