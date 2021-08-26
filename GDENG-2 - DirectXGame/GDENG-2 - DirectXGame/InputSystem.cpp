#include "InputSystem.h"
#include <Windows.h>

#include <iostream>
#include <string>

InputSystem* InputSystem::m_system = nullptr;

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
    InputSystem::m_system = nullptr;
}

void InputSystem::update()
{
    POINT cur_mouse_pos = {};
    ::GetCursorPos(&cur_mouse_pos);

    if (m_first_time) {
        m_old_mouse_pos = Point(cur_mouse_pos.x, cur_mouse_pos.y);

        m_first_time = false;
    }

    if (cur_mouse_pos.x != m_old_mouse_pos.m_x ||
        cur_mouse_pos.y != m_old_mouse_pos.m_y)
        callOnMouseMove(Point(cur_mouse_pos.x, cur_mouse_pos.y));

    m_old_mouse_pos = Point(cur_mouse_pos.x, cur_mouse_pos.y);

    if (::GetKeyboardState(m_keys_state))
    {
        for (unsigned int i = 0; i < 256; i++) {
            
            if (m_keys_state[i] & 0x80) // KEY IS DOWN
            {
                if (i == VK_LBUTTON) {
                    if (m_keys_state[i] != m_old_keys_state[i])
                        callOnLeftMouseDown(Point(cur_mouse_pos.x, cur_mouse_pos.y));
                }
                else if (i == VK_RBUTTON) {
                    if (m_keys_state[i] != m_old_keys_state[i])
                        callOnRightMouseDown(Point(cur_mouse_pos.x, cur_mouse_pos.y));
                }
                else callOnKeyDown(i);
            }
            else if (m_keys_state[i] != m_old_keys_state[i]) // KEY IS UP
            {
                if (i == VK_LBUTTON)
                    callOnLeftMouseUp(Point(cur_mouse_pos.x, cur_mouse_pos.y));
                else if (i == VK_RBUTTON)
                    callOnRightMouseUp(Point(cur_mouse_pos.x, cur_mouse_pos.y));
                else callOnKeyUp(i);
            }
        }

        ::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
    }
}

void InputSystem::addListener(InputListener* listener)
{
    m_set_listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
    m_set_listeners.erase(listener);
}

const Point& InputSystem::getCursorPosition()
{
    POINT cur_mouse_pos = {};
    ::GetCursorPos(&cur_mouse_pos);

    return Point(cur_mouse_pos.x, cur_mouse_pos.y);
}

void InputSystem::setCursorPosition(const Point& pos)
{
    ::SetCursorPos(pos.m_x, pos.m_y);
}

void InputSystem::showCursor(bool show)
{
    if (show) while (::ShowCursor(show) < 0);
    else while (::ShowCursor(show) >= 0);
}

InputSystem* InputSystem::get()
{
    return m_system;
}

void InputSystem::create()
{
    if (InputSystem::m_system)
        throw std::exception("InputSystem already created");

    InputSystem::m_system = new InputSystem();
}

void InputSystem::release()
{
    if (!InputSystem::m_system) return;

    delete InputSystem::m_system;
}

bool InputSystem::isKeyDown(int key)
{
    for (int i = 0; i < 256; i++) {
        if (m_keys_state[i] & 0x80 && i == key) return true;
        else if (i == key) return false;
    }

    return false;
}

bool InputSystem::isKeyUp(int key)
{
    for (int i = 0; i < 256; i++) {
        if (!(m_keys_state[i] & 0x80) && i == key) return true;
        else if (i == key) return false;
    }

    return false;
}

void InputSystem::callOnKeyDown(int key)
{
    std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

    while (it != m_set_listeners.end()){
        (*it)->onKeyDown(key);
        ++it;
    }
}

void InputSystem::callOnKeyUp(int key)
{
    std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

    while (it != m_set_listeners.end()) {
        (*it)->onKeyUp(key);
        ++it;
    }
}

void InputSystem::callOnMouseMove(Point deltaPt)
{
    std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

    while (it != m_set_listeners.end()) {
        (*it)->onMouseMove(deltaPt);
        ++it;
    }
}

void InputSystem::callOnLeftMouseDown(Point deltaPt)
{
    std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

    while (it != m_set_listeners.end()) {
        (*it)->onLeftMouseDown(deltaPt);
        ++it;
    }
}

void InputSystem::callOnLeftMouseUp(Point deltaPt)
{
    std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

    while (it != m_set_listeners.end()) {
        (*it)->onLeftMouseUp(deltaPt);
        ++it;
    }
}

void InputSystem::callOnRightMouseDown(Point deltaPt)
{
    std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

    while (it != m_set_listeners.end()) {
        (*it)->onRightMouseDown(deltaPt);
        ++it;
    }
}

void InputSystem::callOnRightMouseUp(Point deltaPt)
{
    std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

    while (it != m_set_listeners.end()) {
        (*it)->onRightMouseUp(deltaPt);
        ++it;
    }
}
