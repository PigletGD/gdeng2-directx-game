#include "AUIScreen.h"

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
