#pragma once

#define NOMINAX // to avoid conflict with min and max methods of windows.h
#include <windows.h>

class Window
{
public:
	Window();
	~Window();

	bool isRun();

	RECT getClientWindowRect();

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onSize();

private:
	bool broadcast();

protected:
	HWND m_HWND;
	bool m_isRun;
	bool m_is_init = false;
};