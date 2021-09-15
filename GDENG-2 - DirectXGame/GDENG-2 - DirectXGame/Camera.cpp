#include "Camera.h"
#include "AppWindow.h"
#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "EngineTime.h"
#include "MathUtils.h"

#include <iostream>
#include <string>

Camera::Camera(float width, float height)
	: AGameObject("Camera", AGameObject::PrimitiveType::CAMERA)
{
	setPosition(Vector3D(0, 1, -4));

	updateWindowSize(width, height);
}

Camera::~Camera()
{
}

void Camera::updatePosition(float speed, float forward, float rightward)
{
	float moveSpeed = EngineTime::getDeltaTime() * speed;

	Vector3D localPos = getLocalPosition();
	
	Vector3D dir_x, dir_y, dir_z;

	dir_x = m_world_cam.getXDirection() * moveSpeed * rightward;
	if (!m_is_perspective) dir_y = m_world_cam.getYDirection() * moveSpeed * forward;
	else dir_z = m_world_cam.getZDirection() * moveSpeed * forward;
	
	Vector3D new_pos = localPos + dir_x + dir_y + dir_z;

	setPosition(new_pos);

	updateWorldAndViewMatrix();
}

void Camera::updateRotation(float rot_x, float rot_y)
{
	Vector3D localRot = this->getLocalRotation();
	float x = localRot.m_x;
	float y = localRot.m_y;
	float z = localRot.m_z;

	x += rot_x;
	y += rot_y;

	float lookLimit = MathUtils::DegToRad(90);
	if (x > lookLimit) x = lookLimit;
	else if (x < -lookLimit) x = -lookLimit;

	setRotation(x, y, z);
	updateWorldAndViewMatrix();
}

void Camera::updateWorldAndViewMatrix()
{
	m_world_cam.setIdentity();
	
	Matrix4x4 temp;
	temp.setIdentity();

	Vector3D localRot = getLocalRotation();

	temp.setIdentity();
	temp.setRotationX(localRot.m_x);
	m_world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(localRot.m_y);
	m_world_cam *= temp;

	temp.setIdentity();
	temp.setTranslation(getLocalPosition());
	m_world_cam = m_world_cam.multiplyTo(temp);

	temp = m_world_cam;
	temp.inverse();
	m_view_cam = temp;
}

void Camera::updateWindowSize(float width, float height)
{
	m_window_width = width;
	m_window_height = height;
}

void Camera::setToPerspectiveMode(bool value)
{
	m_is_perspective = value;
}

void Camera::switchProjectionMode()
{
	m_is_perspective = !m_is_perspective;
}

void Camera::setOrthographicView()
{
	m_proj_cam.setOrthoLH(m_window_width/100.0f, m_window_height/100.0f, -400.0f, 400.0f);
}

void Camera::setPerspectiveView()
{
	m_proj_cam.setPerspectiveFovLH(m_field_of_view, m_window_width / m_window_height, m_near_clip_plane, m_far_clip_plane);
}

void Camera::setToNormalViewMode()
{
	if (!m_is_in_direction_view) return;

	m_is_in_direction_view = false;

	setPosition(m_retained_pos);
	setRotation(m_retained_rot);

	updateWorldAndViewMatrix();

	m_is_perspective = true;
}

void Camera::setToTopDownViewMode()
{
	if (!m_is_in_direction_view) {
		m_is_in_direction_view = true;

		m_retained_pos = getLocalPosition();
		m_retained_rot = getLocalRotation();
	}

	setRotation(MathUtils::DegToRad(90), 0, 0);

	setPosition(0, getLocalPosition().magnitude(), 0);

	updateWorldAndViewMatrix();

	m_is_perspective = false;
}

void Camera::setToFrontViewMode()
{
	if (!m_is_in_direction_view) {
		m_is_in_direction_view = true;

		m_retained_pos = getLocalPosition();
		m_retained_rot = getLocalRotation();
	}

	setRotation(0, 0, 0);

	setPosition(0, 0, -getLocalPosition().magnitude());

	updateWorldAndViewMatrix();

	m_is_perspective = false;
}

void Camera::setToRighViewMode()
{
	if (!m_is_in_direction_view) {
		m_is_in_direction_view = true;

		m_retained_pos = getLocalPosition();
		m_retained_rot = getLocalRotation();
	}

	setRotation(0, MathUtils::DegToRad(-90), 0);

	setPosition(getLocalPosition().magnitude(), 0, 0);

	updateWorldAndViewMatrix();

	m_is_perspective = false;
}

Matrix4x4 Camera::getWorldMatrix()
{
	return m_world_cam;
}

Matrix4x4 Camera::getViewMatrix()
{
	return m_view_cam;
}

Matrix4x4 Camera::getProjectionMatrix()
{
	if (m_is_perspective) setPerspectiveView();
	else setOrthographicView();

	return m_proj_cam;
}

void Camera::createBuffersAndShaders()
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	constant cc;
	cc.m_time = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	//m_gizmo_icon->createBuffers(shader_byte_code, size_shader);

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
	
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelGizmoShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();
}

void Camera::drawGizmoIcon(constant cc)
{
	Matrix4x4 translation;

	Matrix4x4 temp;
	temp.inverse();

	cc.m_time = 0;
	cc.m_world.flipZBasisVector();
	cc.m_world.setTranslation(m_world_cam.getTranslation());

	DeviceContextPtr dc = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();

	m_cb->update(dc, &cc);

	dc->setConstantBuffer(m_cb);

	dc->setVertexShader(m_vs);
	dc->setPixelShader(m_ps);

	m_gizmo_icon->draw();
}