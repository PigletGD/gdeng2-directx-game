#include "Debug.h"

#include <iostream>
#include <sstream>

Debug* Debug::m_shared_instance = NULL;

void Debug::initialize()
{
	m_shared_instance = new Debug();
}

void Debug::destroy()
{
	delete m_shared_instance;
}

void Debug::Log(String msg)
{
	if (m_shared_instance->m_console != NULL) {
		m_shared_instance->m_console->appendText(msg);
	}
	else {
		m_shared_instance->m_pending_messages.push_back(msg);
	}

	std::cout << msg;
}

void Debug::assignConsole(ConsoleScreen* console)
{
	m_shared_instance->m_console = console;

	for (int i = 0; i < m_shared_instance->m_pending_messages.size(); i++)
		m_shared_instance->m_console->appendText(m_shared_instance->m_pending_messages[i]);

	m_shared_instance->m_pending_messages.clear();
}

Debug::Debug()
{
}

Debug::~Debug()
{
	m_console = NULL;
}