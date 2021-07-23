#include "AppWindow.h"
#include "InputSystem.h"
#include "EngineTime.h"

#pragma comment(lib, "d3d11.lib")

int main()
{	
	{
		try {
			AppWindow::intialize();
			AppWindow* app = (AppWindow*) AppWindow::get();
			app->initializeEngine();

			while (app->isRun());
		}
		catch (...)
		{
			InputSystem::release();
			GraphicsEngine::release();

			return -1;
		}
	}

	InputSystem::release();
	GraphicsEngine::release();

	return 0;
}