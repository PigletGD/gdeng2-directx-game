#pragma once
#include "AUIScreen.h"

class HierarchyScreen : public AUIScreen
{
private:
	HierarchyScreen();
	~HierarchyScreen();

	virtual void drawUI() override;
	void updateObjectList() const;

	char str[128] = {};
	std::string objectString;

	friend class UIManager;
};