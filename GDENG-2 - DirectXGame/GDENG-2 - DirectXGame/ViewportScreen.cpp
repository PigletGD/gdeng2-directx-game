#include "ViewportScreen.h"

#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "UIManager.h"

ViewportScreen::ViewportScreen(String name) :
	AUIScreen(name), m_name(name)
{
	oldSize = ImVec2(1024, 768);
	m_rt = new RenderTexture(1024, 768);
	m_camera = new Camera(1024, 768);

	GraphicsEngine::get()->getCameraSystem()->addNewCamera(m_camera);

	//ImGui::Begin(m_name.c_str());
	//ImGui::Image((void*)m_rt->m_shader_resource_view, ImVec2(1024, 768));
	ImGui::SetNextWindowSize(ImVec2(1024, 768));
	//ImGui::End();
}

ViewportScreen::~ViewportScreen()
{
}

void ViewportScreen::drawUI()
{
	if (!m_is_open)
	{
		UINames uiNames;
		closeWindow(uiNames.VIEWPORT_SCREEN);

		delete this;

		return;
	}

	DeviceContextPtr device_context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();
	SwapChainPtr swap_chain = AppWindow::get()->m_swap_chain;

	/*Render Menu Bar for Viewing Options*/
	//ImGui::Begin("Viewport", &m_is_open);
	if (ImGui::Begin(m_name.c_str(), &m_is_open, ImGuiWindowFlags_MenuBar)) {

		ImVec2 sizes = ImGui::GetContentRegionAvail();

		if (sizes.x != oldSize.x || sizes.y != oldSize.y) {
			m_rt->resize(sizes.x, sizes.y);
			m_camera->updateWindowSize(sizes.x, sizes.y);

			oldSize = sizes;
		}

		//Update Camera in Camera Manager//

		//Ask Render Texture to Update//
		m_rt->clearRenderTarget(device_context, swap_chain, 0.4f, 0.4f, 1.0f, 1.0f);
		m_rt->setRenderTarget(device_context, swap_chain);
		m_rt->setViewportSize(device_context, sizes.x, sizes.y);
		AppWindow::get()->drawToRenderTarget(m_camera, sizes.x, sizes.y); // width and height don't matter

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Viewmode"))
			{
				if (ImGui::MenuItem("Topdown")) {}
				if (ImGui::MenuItem("Front")) {}
				if (ImGui::MenuItem("Right")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Projection"))
			{
				if (ImGui::MenuItem("Perspective")) {}
				if (ImGui::MenuItem("Orthographic")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Fill"))
			{
				if (ImGui::MenuItem("Solid")) {}
				if (ImGui::MenuItem("Wireframe")) {}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//Load Image
		ImGui::Image((void*)m_rt->m_shader_resource_view, sizes);

		ImGui::End();
	}
}