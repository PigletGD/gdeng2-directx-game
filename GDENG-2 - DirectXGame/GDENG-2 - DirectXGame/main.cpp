#include "AppWindow.h"
#include "EngineTime.h"
#include "InputSystem.h"
#pragma comment(lib, "d3d11.lib")

int main()
{	
	{
		try
		{
			GraphicsEngine::create();
			InputSystem::create();
			EngineTime::create();
			AppWindow::create();
			AppWindow* app = AppWindow::get();
			app->initialize();

			while (app->isRun())
				;
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