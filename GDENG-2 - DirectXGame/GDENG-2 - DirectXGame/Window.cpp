#include "Window.h"

#include <iostream>
#include <exception>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE: {
		break; // Event fired when the window is created
	}
	case WM_SETFOCUS: {
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onFocus();
		break; // Event fired when the window gets focused
	}
	case WM_KILLFOCUS: {
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break; // Event fired when the window gets out of focused
	}
	case WM_DESTROY: {
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break; // Event fired when the window is destroyed
	}
	default: return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

Window::Window()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyClassWindow";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc)) throw std::exception("Window not created successfully");

	m_HWND = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyClassWindow",
		L"DirectX Application", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);

	if (!m_HWND) throw std::exception("Window not created successfully");

	::ShowWindow(m_HWND, SW_SHOW);
	::UpdateWindow(m_HWND);

	m_isRun = true;
}

Window::~Window()
{
	
}

bool Window::broadcast()
{
	MSG msg;

	if (!this->m_is_init)
	{
		SetWindowLongPtr(m_HWND, GWLP_USERDATA, (LONG_PTR)this);
		this->onCreate();
		this->m_is_init = true;
	}

	this->onUpdate();

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(1);

	return true;
}

bool Window::isRun()
{
	if (m_isRun) broadcast();

	return m_isRun;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_HWND, &rc);
	return rc;
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	m_isRun = false;
}

void Window::onFocus()
{

}

void Window::onKillFocus()
{

}
