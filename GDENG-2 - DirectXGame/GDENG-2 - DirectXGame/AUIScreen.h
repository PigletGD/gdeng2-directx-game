#pragma once
#include <string>

typedef std::string String;
class UIManager;

class AUIScreen
{
protected:
	AUIScreen(String name);
	~AUIScreen();

	String getName();
	virtual void drawUI() = 0;

	String m_name;

	friend class UIManager;
};