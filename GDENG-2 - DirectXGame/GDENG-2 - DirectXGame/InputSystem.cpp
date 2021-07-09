#include "InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update()
{
    if (::GetKeyboardState(m_keys_state))
    {
        for (unsigned int i = 0; i < 256; i++) {
            
            if (m_keys_state[i] & 0x80) // KEY IS DOWN
            {
                std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

                while (it != m_set_listeners.end())
                {
                    (*it)->onKeyDown(i);
                    ++it;
                }
            }
            else if (m_keys_state[i] != m_old_keys_state[i]) // KEY IS UP
            {
                std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
                    
                while (it != m_set_listeners.end())
                {
                    (*it)->onKeyUp(i);
                    ++it;
                }
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

InputSystem* InputSystem::get()
{
    static InputSystem system;

    return &system;
}
