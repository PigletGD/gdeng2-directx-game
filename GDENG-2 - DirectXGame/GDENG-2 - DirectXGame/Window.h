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

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

protected:
	HWND m_HWND;
	bool m_isRun;
};