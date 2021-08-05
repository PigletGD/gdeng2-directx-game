#include "CameraSystem.h"
#include "AppWindow.h"
#include "InputSystem.h"
#include "EngineTime.h"

#include <iostream>

CameraSystem::CameraSystem()
{
	cameraList.push_back(new Camera());
}

CameraSystem::~CameraSystem()
{
	
}

void CameraSystem::addNewCamera()
{
	std::cout << "Added New Camera to Scene" << std::endl;

	Camera* camera = new Camera();
	camera->createBuffersAndShaders();
	camera->updatePosition(m_cam_speed, m_forward, m_rightward);

	auto it = cameraList.begin() + m_camera_index + 1;

	cameraList.insert(it, camera);

	switchToNextCamera();
}

void CameraSystem::removeCurrentCamera()
{
	if (cameraList.size() <= 1) return;

	std::cout << "Removed Current Camera from Scene" << std::endl;

	auto it = cameraList.begin() + m_camera_index;

	cameraList.erase(it);

	if (m_camera_index >= cameraList.size())
		m_camera_index = 0;
}

void CameraSystem::switchToPreviousCamera()
{
	std::cout << "Switched from Camera " << std::to_string(m_camera_index);
	m_camera_index--;

	if (m_camera_index < 0)
		m_camera_index = cameraList.size() - 1;

	std::cout << " to " << std::to_string(m_camera_index) << std::endl;
}

void CameraSystem::switchToNextCamera()
{
	std::cout << "Switched from Camera " << std::to_string(m_camera_index);
	m_camera_index++;

	if (m_camera_index >= cameraList.size())
		m_camera_index = 0;

	std::cout << " to " << std::to_string(m_camera_index) << std::endl;
}

void CameraSystem::updateCurrentCamera()
{
	if (m_forward != 0 || m_rightward != 0)
		cameraList[m_camera_index]->updatePosition(
			m_cam_speed, m_forward, m_rightward);
}

Matrix4x4 CameraSystem::getCurrentCameraWorldMatrix()
{
	return cameraList[m_camera_index]->getWorldMatrix();
}

Matrix4x4 CameraSystem::getCurrentCameraViewMatrix()
{
	return cameraList[m_camera_index]->getViewMatrix();
}

Matrix4x4 CameraSystem::getCurrentCameraProjectionMatrix()
{
	return cameraList[m_camera_index]->getProjectionMatrix();
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
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_gizmo_ps, m_gizmo_icon);

	for (int i = 0; i < cameraList.size(); i++) {
		if (i != m_camera_index)
			cameraList[i]->drawGizmoIcon(cc);
	}
}

void CameraSystem::onKeyDown(int key)
{
	if (key == 'W') m_forward = 1.0f;
	else if (key == 'S') m_forward = -1.0f;
	else if (key == 'A') m_rightward = -1.0f;
	else if (key == 'D') m_rightward = 1.0f;

	else if (key == m_add_cam_key && !m_pressed_add_cam) {
		addNewCamera();
		m_pressed_add_cam = true;
	}
	else if (key == m_remove_cam_key && !m_pressed_remove_cam) {
		removeCurrentCamera();
		m_pressed_remove_cam = true;
	}
	else if (key == m_prev_cam_key && !m_pressed_prev_cam) {
		switchToPreviousCamera();
		m_pressed_prev_cam = true;
	}
	else if (key == m_next_cam_key && !m_pressed_next_cam) {
		switchToNextCamera();
		m_pressed_next_cam = true;
	}
	else if (key == m_switch_proj_key && !m_pressed_switch_proj) {
		std::cout << "Switching Projection Mode" << std::endl;
		cameraList[m_camera_index]->switchProjectionMode();
		m_pressed_switch_proj = true;
	}
}

void CameraSystem::onKeyUp(int key)
{
	if (key == 'W' || key == 'S') m_forward = 0.0f;
	else if (key == 'A' || key == 'D') m_rightward = 0.0f;
	else if (key == m_add_cam_key) m_pressed_add_cam = false;
	else if (key == m_remove_cam_key) m_pressed_remove_cam = false;
	else if (key == m_prev_cam_key) m_pressed_prev_cam = false;
	else if (key == m_next_cam_key) m_pressed_next_cam = false;
	else if (key == m_switch_proj_key) m_pressed_switch_proj = false;
}

void CameraSystem::onMouseMove(const Point& mouse_pos)
{
	if (!m_pressed_rotate_cam) return;

	AppWindow* app = AppWindow::get();

	float dt = EngineTime::getDeltaTime();

	float delta_rot_x = (mouse_pos.m_y - m_old_mouse_pos.m_y) * dt * 0.2f;
	float delta_rot_y = (mouse_pos.m_x - m_old_mouse_pos.m_x) * dt * 0.2f;

	InputSystem::get()->setCursorPosition(m_old_mouse_pos);

	cameraList[m_camera_index]->updateRotation(delta_rot_x, delta_rot_y);
}

void CameraSystem::onLeftMouseDown(const Point& mouse_pos)
{
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