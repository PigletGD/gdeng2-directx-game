#include "Window.h"
#include "EngineTime.h"
#include "imgui.h"
#include "UIManager.h"
#include <iostream>
#include <exception>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	switch (msg)
	{
	case WM_CREATE: {
		break; // Event fired when the window is created
	}
	case WM_SIZE: {
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onSize();
		break; // Event fired when the window is resized
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
	UIManager::destroy();
}

bool Window::broadcast()
{
	EngineTime::LogFrameStart();

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

	Sleep(0);

	EngineTime::LogFrameEnd();

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

void Window::onSize()
{
}
