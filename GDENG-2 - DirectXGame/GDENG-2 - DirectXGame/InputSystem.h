#pragma once
#include "InputListener.h"
#include <unordered_set>
#include "Point.h"

class InputSystem
{
private:
	InputSystem();
	~InputSystem();

public:
	void update();

	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	const Point& getCursorPosition();
	void setCursorPosition(const Point& pos);

	void showCursor(bool show);

	static InputSystem* get();
	static void create();
	static void release();

	bool isKeyDown(int key);
	bool isKeyUp(int key);

private:
	static InputSystem* m_system;

	std::unordered_set<InputListener*> m_set_listeners;

	void callOnKeyDown(int key);
	void callOnKeyUp(int key);

	void callOnMouseMove(Point deltaPt);
	void callOnLeftMouseDown(Point deltaPt);
	void callOnLeftMouseUp(Point deltaPt);
	void callOnRightMouseDown(Point deltaPt);
	void callOnRightMouseUp(Point deltaPt);

	unsigned char m_old_keys_state[256] = {};
	unsigned char m_keys_state[256] = {};

	Point m_old_mouse_pos;

	bool m_first_time = true;
};