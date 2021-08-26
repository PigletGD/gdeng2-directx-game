#include "CameraSystem.h"
#include "AppWindow.h"
#include "InputSystem.h"
#include "EngineTime.h"

#include <iostream>

CameraSystem::CameraSystem()
{
	AppWindow* app = AppWindow::get();
	int width = (app->getClientWindowRect().right - app->getClientWindowRect().left);
	int height = (app->getClientWindowRect().bottom - app->getClientWindowRect().top);

	cameraList.push_back(new Camera(width, height));
	cameraList[0]->m_is_active = true;
}

CameraSystem::~CameraSystem()
{
	
}

Camera* CameraSystem::getCurrentCamera()
{
	return cameraList[m_view_camera_index];
}

void CameraSystem::addNewCamera()
{
	std::cout << "Added New Camera to Scene" << std::endl;

	AppWindow* app = AppWindow::get();
	int width = (app->getClientWindowRect().right - app->getClientWindowRect().left);
	int height = (app->getClientWindowRect().bottom - app->getClientWindowRect().top);

	Camera* camera = new Camera(width, height);
	camera->createBuffersAndShaders();
	camera->updatePosition(m_cam_speed, m_forward, m_rightward);

	auto it = cameraList.begin() + m_control_camera_index + 1;

	cameraList.insert(it, camera);

	switchToNextCamera();
}

void CameraSystem::addNewCamera(Camera* new_camera)
{
	std::cout << "Added New Camera to Scene" << std::endl;

	new_camera->createBuffersAndShaders();
	new_camera->updatePosition(m_cam_speed, m_forward, m_rightward);
	new_camera->m_is_active = true;

	auto it = cameraList.begin() + m_control_camera_index + 1;

	m_viewport_camera_count++;

	cameraList.insert(it, new_camera);
}

void CameraSystem::removeCurrentCamera()
{
	if (cameraList.size() <= 1) return;

	std::cout << "Removed Current Camera from Scene" << std::endl;

	auto it = cameraList.begin() + m_view_camera_index;

	cameraList.erase(it);

	if (m_view_camera_index >= cameraList.size())
		m_view_camera_index = 0;
}

void CameraSystem::removeCamera(Camera* camera)
{
	// Find camera to remove in camera list
	auto itr = std::find(cameraList.begin(), cameraList.end(), camera);
	int camera_index = std::distance(cameraList.begin(), itr);

	// Don't execute if trying to remove main view camera and it is the only available main view camera left
	if (m_view_camera_index == camera_index &&
		cameraList.size() - m_viewport_camera_count <= 1) {
		std::cout << std::to_string(cameraList.size() - m_viewport_camera_count) << "Can't remove camera" << std::endl;
		return;
	}

	// If found camera
	if (itr != cameraList.end()) {
		// Deduct counter if trying to remove a viewport camera
		if (cameraList[camera_index]->m_is_active && camera_index != m_view_camera_index) {
			std::cout << "Removed Viewport Camera" << std::endl;
			m_viewport_camera_count--;
		}

		// Erase camera from list
		cameraList.erase(itr);

		// If removed camera has an index below the main view camera
		// shift main view camera index to retain view
		if (camera_index < m_view_camera_index) m_view_camera_index--;
		else if (camera_index == m_view_camera_index) {
			// Find another camera to act as main view camera
			// to replace main view camera
			m_view_camera_index--;

			if (m_view_camera_index < 0) {
				m_view_camera_index = cameraList.size() - 1;
			}

			// Finding camera that is not a viewport camera
			while (cameraList[m_view_camera_index]->m_is_active) {
				m_view_camera_index--;

				if (m_view_camera_index < 0)
					m_view_camera_index = cameraList.size() - 1;
			}

			cameraList[m_view_camera_index]->m_is_active = true;
		}

		// If removed camera has an index below the controlled camera
		// shift controlled camera index to retain control on same camera
		if (camera_index < m_control_camera_index) m_control_camera_index--;
		else if (camera_index == m_control_camera_index) {
			// Find another camera to control if camera being deleted
			// is the camera that is currently being controlled
			m_control_camera_index--;

			if (m_control_camera_index < 0)
				m_control_camera_index = cameraList.size() - 1;

			// Finding a camera that is currently active/rendering in editor
			while (!cameraList[m_control_camera_index]->m_is_active) {
				m_control_camera_index--;

				if (m_control_camera_index < 0)
					m_control_camera_index = cameraList.size() - 1;
			}
		}

		delete camera;

		std::cout << "Successfully removed camera" << std::endl;
	}
	else std::cout << "Could not delete camera" << std::endl;
}

void CameraSystem::switchCamera(Camera* camera)
{
	// Find camera to remove in camera list
	auto itr = std::find(cameraList.begin(), cameraList.end(), camera);

	if (itr != cameraList.end()) {
		std::cout << "Switched from Camera " << std::to_string(m_control_camera_index);

		int camera_index = std::distance(cameraList.begin(), itr);

		m_control_camera_index = camera_index;

		std::cout << " to " << std::to_string(m_control_camera_index) << std::endl;
	}
	else std::cout << "Could not switch camera" << std::endl;
}

void CameraSystem::switchToPreviousCamera()
{
	std::cout << "Switched from Camera " << std::to_string(m_control_camera_index);
	int old_index = m_view_camera_index;
	m_control_camera_index--;

	if (m_control_camera_index < 0)
		m_control_camera_index = cameraList.size() - 1;

	if (!cameraList[m_control_camera_index]->m_is_active) {
		cameraList[old_index]->m_is_active = false;
		cameraList[m_control_camera_index]->m_is_active = true;
		m_view_camera_index = m_control_camera_index;
	}

	std::cout << " to " << std::to_string(m_control_camera_index) << std::endl;
}

void CameraSystem::switchToNextCamera()
{
	std::cout << "Switched from Camera " << std::to_string(m_control_camera_index);
	int old_index = m_view_camera_index;
	m_control_camera_index++;

	if (m_control_camera_index >= cameraList.size())
		m_control_camera_index = 0;

	if (!cameraList[m_control_camera_index]->m_is_active) {
		cameraList[old_index]->m_is_active = false;
		cameraList[m_control_camera_index]->m_is_active = true;
		m_view_camera_index = m_control_camera_index;
	}

	std::cout << " to " << std::to_string(m_control_camera_index) << std::endl;
}

void CameraSystem::updateCurrentCamera()
{
	if (m_forward != 0 || m_rightward != 0)
		cameraList[m_control_camera_index]->updatePosition(
			m_cam_speed, m_forward, m_rightward);
}

void CameraSystem::updateCurrentCameraWindowSize(float width, float height)
{
	cameraList[m_view_camera_index]->updateWindowSize(width, height);
}

Matrix4x4 CameraSystem::getCurrentCameraWorldMatrix()
{
	return cameraList[m_view_camera_index]->getWorldMatrix();
}

Matrix4x4 CameraSystem::getCurrentCameraViewMatrix()
{
	return cameraList[m_view_camera_index]->getViewMatrix();
}

Matrix4x4 CameraSystem::getCurrentCameraProjectionMatrix()
{
	return cameraList[m_view_camera_index]->getProjectionMatrix();
}

void CameraSystem::initializeGizmoTexture()
{
	m_gizmo_icon = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\camera_icon.png");
}

void CameraSystem::initializeInitialCamera()
{
	InputSystem* input_system = InputSystem::get();

	m_old_mouse_pos = input_system->getCursorPosition();
	
	for (int i = 0; i < cameraList.size(); i++) {
		cameraList[i]->createBuffersAndShaders();
		cameraList[i]->updatePosition(m_cam_speed, m_forward, m_rightward);
	}		
}

void CameraSystem::drawGizmos(constant cc)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_gizmo_icon);

	for (int i = 0; i < cameraList.size(); i++) {
		if (i != m_view_camera_index)
			cameraList[i]->drawGizmoIcon(cc);
	}
}

void CameraSystem::setHoverViewportState(bool value)
{
	m_is_over_viewport = value;
}

void CameraSystem::incrementFocusCount()
{
	m_window_focus_count++;

	std::cout << "Increased focus count: " << std::to_string(m_window_focus_count) << std::endl;
}

void CameraSystem::decrementFocusCount()
{
	if (m_window_focus_count <= 0) return;

	m_window_focus_count--;

	std::cout << "Decreased focus count: " << std::to_string(m_window_focus_count) << std::endl;
}

void CameraSystem::updateInputListener()
{
	if (!m_is_listening && m_window_focus_count > 0) {
		InputSystem::get()->addListener(this);
		m_is_listening = true;
	}
	else if (m_is_listening && m_window_focus_count <= 0) {
		InputSystem::get()->removeListener(this);
		m_is_listening = false;
	}
}

void CameraSystem::onKeyDown(int key)
{
	if (key == 'W') m_forward = 1.0f;
	else if (key == 'S') m_forward = -1.0f;
	else if (key == 'A') m_rightward = -1.0f;
	else if (key == 'D') m_rightward = 1.0f;

	else if (key == m_prev_cam_key && !m_pressed_prev_cam) {
		switchToPreviousCamera();
		m_pressed_prev_cam = true;
	}
	else if (key == m_next_cam_key && !m_pressed_next_cam) {
		switchToNextCamera();
		m_pressed_next_cam = true;
	}
	/*
	else if (key == m_add_cam_key && !m_pressed_add_cam) {
		addNewCamera();
		m_pressed_add_cam = true;
	}
	else if (key == m_remove_cam_key && !m_pressed_remove_cam) {
		removeCamera(cameraList[m_view_camera_index]);
		m_pressed_remove_cam = true;
	}
	else if (key == m_switch_proj_key && !m_pressed_switch_proj) {
		std::cout << "Switching Projection Mode" << std::endl;
		cameraList[m_control_camera_index]->switchProjectionMode();
		m_pressed_switch_proj = true;
	}
	*/
}

void CameraSystem::onKeyUp(int key)
{
	if (key == 'W' || key == 'S') m_forward = 0.0f;
	else if (key == 'A' || key == 'D') m_rightward = 0.0f;
	else if (key == m_prev_cam_key) m_pressed_prev_cam = false;
	else if (key == m_next_cam_key) m_pressed_next_cam = false;
	//else if (key == m_add_cam_key) m_pressed_add_cam = false;
	//else if (key == m_remove_cam_key) m_pressed_remove_cam = false;
	//else if (key == m_switch_proj_key) m_pressed_switch_proj = false;
}

void CameraSystem::onMouseMove(const Point& mouse_pos)
{
	if (!m_pressed_rotate_cam) return;

	AppWindow* app = AppWindow::get();

	float dt = EngineTime::getDeltaTime();

	float delta_rot_x = (mouse_pos.m_y - m_old_mouse_pos.m_y) * dt * 0.2f;
	float delta_rot_y = (mouse_pos.m_x - m_old_mouse_pos.m_x) * dt * 0.2f;

	InputSystem::get()->setCursorPosition(m_old_mouse_pos);

	cameraList[m_control_camera_index]->updateRotation(delta_rot_x, delta_rot_y);
}

void CameraSystem::onLeftMouseDown(const Point& mouse_pos)
{
	if (m_is_over_viewport) return;

	std::cout << "Switched from Camera " << std::to_string(m_control_camera_index);

	m_control_camera_index = m_view_camera_index;

	std::cout << " to " << std::to_string(m_control_camera_index) << std::endl;
}

void CameraSystem::onLeftMouseUp(const Point& mouse_pos)
{
}

void CameraSystem::onRightMouseDown(const Point& mouse_pos)
{
	InputSystem* input_system = InputSystem::get();

	m_old_mouse_pos = input_system->getCursorPosition();
	input_system->showCursor(false);

	m_pressed_rotate_cam = true;
}

void CameraSystem::onRightMouseUp(const Point& mouse_pos)
{
	InputSystem* input_system = InputSystem::get();

	input_system->setCursorPosition(m_old_mouse_pos);
	input_system->showCursor(true);

	m_pressed_rotate_cam = false;
}