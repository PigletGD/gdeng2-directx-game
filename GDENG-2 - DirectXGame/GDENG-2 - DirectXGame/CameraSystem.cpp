#include "CameraSystem.h"
#include "AppWindow.h"
#include "InputSystem.h"
#include "EngineTime.h"

#include <iostream>

CameraSystem::CameraSystem()
{
	cameraList.push_back(new Camera());
	cameraList.push_back(new Camera());
	cameraList.push_back(new Camera());
	cameraList.push_back(new Camera());
}

CameraSystem::~CameraSystem()
{
	
}

void CameraSystem::initializeGizmoTexture()
{
	m_gizmo_icon = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\camera_icon.png");
}

void CameraSystem::createCameraBuffers(void* shader_byte_code, UINT size_byte_shader)
{
	for (int i = 0; i < cameraList.size(); i++)
		cameraList[i]->createBuffers(shader_byte_code, size_byte_shader);
}

void CameraSystem::setCameraShaders()
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_gizmo_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	createCameraBuffers(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelGizmoShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_gizmo_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();
}

void CameraSystem::switchToPreviousCamera()
{
	m_camera_index--;

	if (m_camera_index < 0)
		m_camera_index = cameraList.size() - 1;
}

void CameraSystem::switchToNextCamera()
{
	m_camera_index++;

	if (m_camera_index >= cameraList.size())
		m_camera_index = 0;
}

void CameraSystem::updateCurrentCamera()
{
	AppWindow* app = AppWindow::get();
	
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(cameraList[m_camera_index]->getXRot());
	world_cam *= temp;
	temp.setIdentity();
	temp.setRotationY(cameraList[m_camera_index]->getYRot());
	world_cam *= temp;

	Vector3D new_pos = cameraList[m_camera_index]->getWorldCameraTranslation() +
		world_cam.getZDirection() * (m_forward * EngineTime::getDeltaTime()) +
		world_cam.getXDirection() * (m_rightward * EngineTime::getDeltaTime());

	world_cam.setTranslation(new_pos);

	cameraList[m_camera_index]->setWorldCameraMatrix(world_cam);
}

Matrix4x4 CameraSystem::getCurrentCameraWorld()
{
	return cameraList[m_camera_index]->getWorldCameraMatrix();
}

Matrix4x4 CameraSystem::getCurrentCameraView()
{
	Matrix4x4 view;

	view = cameraList[m_camera_index]->getWorldCameraMatrix();
	view.inverse();

	return view;
}

Matrix4x4 CameraSystem::getCurrentCameraProjection()
{
	return cameraList[m_camera_index]->getProjection();
}

void CameraSystem::drawGizmos(const VertexShaderPtr& vs, const PixelShaderPtr& ps, constant cc)
{
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(ps, m_gizmo_icon);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_gizmo_ps, m_gizmo_icon);

	for (int i = 0; i < cameraList.size(); i++) {
		if (i != m_camera_index)
			cameraList[i]->drawGizmoIcon(m_gizmo_vs, m_gizmo_ps, cc);
			//cameraList[i]->drawGizmoIcon(vs, ps, cc);
	}
}

void CameraSystem::onKeyDown(int key)
{
	if (key == 'W') m_forward = 1.0f;
	else if (key == 'S') m_forward = -1.0f;
	else if (key == 'A') m_rightward = -1.0f;
	else if (key == 'D') m_rightward = 1.0f;

	else if (key == 'Q' && !m_pressed_q) {
		switchToPreviousCamera();
		m_pressed_q = true;
	}
	else if (key == 'E' && !m_pressed_e) {
		switchToNextCamera();
		m_pressed_e = true;
	}

	else if (key == 'M' && !m_pressed_m) {
		cameraList[m_camera_index]->switchProjectionMode();
		m_pressed_m = true;
	}
}

void CameraSystem::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == 'Q') m_pressed_q = false;
	else if (key == 'E') m_pressed_e = false;
	else if (key == 'M') m_pressed_m = false;
}

void CameraSystem::onMouseMove(const Point& mouse_pos)
{
	AppWindow* app = AppWindow::get();
	int width = (app->getClientWindowRect().right - app->getClientWindowRect().left);
	int height = (app->getClientWindowRect().bottom - app->getClientWindowRect().top);

	float delta_rot_x = (mouse_pos.m_y - (height * 0.5f)) * EngineTime::getDeltaTime() * 0.2f;
	float delta_rot_y = (mouse_pos.m_x - (width * 0.5f)) * EngineTime::getDeltaTime() * 0.2f;

	cameraList[m_camera_index]->updateRotation(delta_rot_x, delta_rot_y);

	InputSystem::get()->setCursorPosition(Point((int)(width * 0.5f), (int)(height * 0.5f)));
}

void CameraSystem::onLeftMouseDown(const Point& mouse_pos)
{
}

void CameraSystem::onLeftMouseUp(const Point& mouse_pos)
{
}

void CameraSystem::onRightMouseDown(const Point& mouse_pos)
{
}

void CameraSystem::onRightMouseUp(const Point& mouse_pos)
{
}
