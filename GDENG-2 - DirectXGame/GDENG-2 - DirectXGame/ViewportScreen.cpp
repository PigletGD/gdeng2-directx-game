#include "ViewportScreen.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "SwapChain.h"

ViewportScreen::ViewportScreen() :
	AUIScreen("Viewport Screen")
{
	m_rt = new RenderTexture(1024, 768);
	m_camera = new Camera();

	GraphicsEngine::get()->getCameraSystem()->addNewCamera(m_camera);
}

ViewportScreen::~ViewportScreen()
{
}

void ViewportScreen::drawUI()
{
	DeviceContextPtr device_context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();
	SwapChainPtr swap_chain = AppWindow::get()->m_swap_chain;

	/*Render Menu Bar for Viewing Options*/
	ImGui::Begin("Viewport");

	/*Update Camera in Camera Manager*/

	/*Ask Render Texture to Update*/
	m_rt->SetRenderTarget(device_context, swap_chain);
	m_rt->ClearRenderTarget(device_context, swap_chain, 0.4f, 0.4f, 1.0f, 1.0f);
	AppWindow::get()->drawToRenderTarget(m_camera);

	/*Load Image*/
	ImGui::Image((void*)m_rt->m_shader_resource_view, ImVec2(1024, 768));

	ImGui::End();
}