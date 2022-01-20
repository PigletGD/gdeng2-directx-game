#include "ViewportScreen.h"

#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "UIManager.h"

ViewportScreen::ViewportScreen(String name, int index) :
	AUIScreen(name), m_name(name)
{
	m_window_name = "Viewport " + std::to_string(index);

	m_old_size = ImGui::GetContentRegionAvail();
	m_camera = new Camera(m_old_size.x, m_old_size.y);

	try {
		m_rt = new RenderTexture(m_old_size.x, m_old_size.y);
	}
	catch (...) {}

	GraphicsEngine::get()->getCameraSystem()->addNewCamera(m_camera);

	m_rs = GraphicsEngine::get()->getRenderSystem()->m_rs_solid;
}

ViewportScreen::~ViewportScreen()
{
}

void ViewportScreen::drawUI()
{
	// Window closing Behaviour
	if (!m_is_open)
	{
		GraphicsEngine::get()->getCameraSystem()->removeCamera(m_camera);

		closeWindow(m_name);

		delete this;

		return;
	}

	DeviceContextPtr device_context = GraphicsEngine::get()->
		getRenderSystem()->getImmediateDeviceContext();
	SwapChainPtr swap_chain = AppWindow::get()->m_swap_chain;

	// Sets view mode to either solid or wireframe
	device_context->setRasterizerState(m_rs);

	GraphicsEngine::get()->getCameraSystem()->setCurrentCamera(m_camera);

	// Initializes Window
	if (ImGui::Begin(m_window_name.c_str(), &m_is_open, ImGuiWindowFlags_MenuBar)) {
		// Checks if window was resized
		ImVec2 new_size = ImGui::GetContentRegionAvail();

		if (new_size.x != m_old_size.x || new_size.y != m_old_size.y) {
			m_rt->resize(new_size.x, new_size.y);
			m_camera->updateWindowSize(new_size.x, new_size.y);

			m_old_size = new_size;
		}

		//Ask Render Texture to Update//
		m_rt->clearRenderTarget(device_context, 1.0f, 0.4f, 0.4f, 1.0f);
		m_rt->setViewportSize(device_context, new_size.x, new_size.y);
		AppWindow::get()->drawToRenderTarget(m_camera, new_size.x, new_size.y);

		// Menu bar that holds buttons that control viewport view modes
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Viewmode"))
			{
				if (ImGui::MenuItem("Topdown")) { m_camera->setToTopDownViewMode(); }
				if (ImGui::MenuItem("Front")) { m_camera->setToFrontViewMode(); }
				if (ImGui::MenuItem("Right")) { m_camera->setToRighViewMode(); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Projection"))
			{
				if (ImGui::MenuItem("Perspective")) { m_camera->setToPerspectiveMode(true); }
				if (ImGui::MenuItem("Orthographic")) { m_camera->setToPerspectiveMode(false); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Fill"))
			{
				if (ImGui::MenuItem("Solid")) { m_rs = GraphicsEngine::get()->getRenderSystem()->m_rs_solid; }
				if (ImGui::MenuItem("Wireframe")) { m_rs = GraphicsEngine::get()->getRenderSystem()->m_rs_wireframe; }
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//Load Image
		ImGui::Image((void*)m_rt->m_srv, new_size);

		// Handles Viewport Focus and Selection
		if (ImGui::IsWindowHovered()) {
			
			if (ImGui::IsItemClicked(1)) ImGui::SetWindowFocus();

			m_is_hovered = true;
		}
		else {
			if (m_is_hovered) {
				m_is_hovered = false;
			}
		}
		if (ImGui::IsWindowFocused()) {
			if (!m_is_focused) {
				CameraSystem* camera_system = GraphicsEngine::get()->getCameraSystem();
				
				m_is_focused = true;

				GraphicsEngine::get()->getCameraSystem()->switchCamera(m_camera);
			}
		}
		else if (m_is_focused) {
			m_is_focused = false;
		}

		ImGui::End();
	}
}