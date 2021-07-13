#include "AppWindow.h"

#pragma comment(lib, "d3d11.lib")

#include <iostream>

int main()
{
	AppWindow::initialize();
	AppWindow* runningApp = (AppWindow*) AppWindow::get();
	runningApp->initializeEngine();

	while (runningApp->isRun())
		runningApp->broadcast();

	return 0;
}