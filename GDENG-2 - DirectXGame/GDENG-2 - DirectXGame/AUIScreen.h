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
	void closeWindow(String uiName);

	virtual void drawUI() = 0;

	String m_name;

	bool m_is_open = true;

	friend class UIManager;
};