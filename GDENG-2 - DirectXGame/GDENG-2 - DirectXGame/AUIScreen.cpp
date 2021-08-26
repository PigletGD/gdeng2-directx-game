#include "AUIScreen.h"

#include "UIManager.h"

#include <algorithm>
#include <iostream>
#include <vector>

AUIScreen::AUIScreen(String name) :
	m_name(name)
{
	
}

AUIScreen::~AUIScreen()
{
}

String AUIScreen::getName()
{
	return m_name;
}

void AUIScreen::closeWindow(String uiName)
{
	UIManager* uiManager = UIManager::getInstance();

	auto itr = std::find(uiManager->uiList.begin(), uiManager->uiList.end(), this);

	if (itr != uiManager->uiList.end()) {
		uiManager->uiList.erase(itr);
		uiManager->uiTable[uiName] = nullptr;

		std::cout << "Successfully closed window" << std::endl;
	}
	else std::cout << "Could not close window" << std::endl;
}