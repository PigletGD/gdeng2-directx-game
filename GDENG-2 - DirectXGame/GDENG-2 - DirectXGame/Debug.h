#pragma once

#include <string>
#include <vector>

#include "ConsoleScreen.h"

class Debug
{
public:
	typedef std::string String;
	typedef std::vector<String> PendingList;

	static void initialize();
	static void destroy();
	static void Log(String msg);
	static void assignConsole(ConsoleScreen* console);

private:
	Debug();
	~Debug();
	Debug(Debug const&) {};             // copy constructor is private
	Debug& operator=(Debug const&) {};  // assignment operator is private*/

	static Debug* m_shared_instance;

	ConsoleScreen* m_console;
	PendingList m_pending_messages;
};