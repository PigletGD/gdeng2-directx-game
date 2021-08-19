#include "ViewportScreen.h"

#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "SwapChain.h"

ViewportScreen::ViewportScreen() :
	AUIScreen("Viewport Screen")
{
	oldSize = ImGui::GetContentRegionAvail();
	m_rt = new RenderTexture(oldSize.x, oldSize.y);
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

	//
	//ImVec2 size = ImGui::GetWindowSize();
	ImVec2 sizes = ImGui::GetContentRegionAvail();

	if (sizes.x != oldSize.x || sizes.y != oldSize.y) {
		m_rt->resize(sizes.x, sizes.y);

		oldSize = sizes;
	}

	//Update Camera in Camera Manager//

	//Ask Render Texture to Update//
	m_rt->clearRenderTarget(device_context, swap_chain, 0.4f, 0.4f, 1.0f, 1.0f);
	m_rt->setRenderTarget(device_context, swap_chain);
	m_rt->setViewportSize(device_context, sizes.x, sizes.y);
	AppWindow::get()->drawToRenderTarget(m_camera, sizes.x, sizes.y); // width and height don't matter

	//Load Image
	ImGui::Image((void*)m_rt->m_shader_resource_view, sizes);

	ImGui::End();
}