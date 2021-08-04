#include "Camera.h"
#include "AppWindow.h"
#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"

#include <iostream>
#include <string>

Camera::Camera() : AGameObject("Camera")
{
	setPosition(Vector3D(0, 1, -2));
	m_world_cam.setTranslation(Vector3D(0, 1, -2));

	m_gizmo_icon = new Quad({Vector3D(-0.05f,-0.05f, 0.0f), Vector2D(1, 1)},
							{Vector3D(-0.05f, 0.05f, 0.0f), Vector2D(1, 0)},
							{Vector3D( 0.05f, 0.05f, 0.0f), Vector2D(0, 0)},
							{Vector3D( 0.05f,-0.05f, 0.0f), Vector2D(0, 1)});
}

Camera::~Camera()
{
}

void Camera::updatePosition(float deltaTime, float speed, float forward, float rightward)
{
	float moveSpeed = deltaTime * speed;

	Vector3D localPos = getLocalPosition();
	
	Vector3D dir_z = m_world_cam.getZDirection() * moveSpeed * forward;
	Vector3D dir_x = m_world_cam.getXDirection() * moveSpeed * rightward;
	
	Vector3D new_pos = localPos + dir_x + dir_z;

	setPosition(new_pos);
	updateWorldAndViewMatrix();
}

void Camera::updateRotation(float rot_x, float rot_y)
{
	Vector3D localRot = this->getLocalRotation();
	float x = localRot.m_x;
	float y = localRot.m_y;
	float z = localRot.m_z;

	//float speed = 0.005f;
	x += rot_x;
	y += rot_y;

	if (x > 1.6023f) x = 1.6023f;
	if (x < -1.6023f) x = -1.6023f;

	this->setRotation(x, y, z);
	this->updateWorldAndViewMatrix();

	/*
	m_rot_x += rot_x;
	m_rot_y += rot_y;

	if (m_rot_x > 1.6023f) m_rot_x = 1.6023f;
	if (m_rot_x < -1.6023f) m_rot_x = -1.6023f;
	*/
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

void Camera::createBuffers(void* shader_byte_code, UINT size_byte_shader)
{
	m_gizmo_icon->createBuffers(shader_byte_code, size_byte_shader);

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

void Camera::setWorldCameraMatrix(Matrix4x4 world_cam)
{
	m_world_cam = world_cam;
}

Matrix4x4 Camera::getWorldCameraMatrix()
{
	return m_world_cam;
}

void Camera::setWorldCameraTranslation(Vector3D new_pos)
{
	m_world_cam.setTranslation(new_pos);
}

Vector3D Camera::getWorldCameraTranslation()
{
	return m_world_cam.getTranslation();
}

float Camera::getXRot()
{
	return m_rot_x;
}

float Camera::getYRot()
{
	return m_rot_y;
}

void Camera::drawGizmoIcon(const VertexShaderPtr& vs, const PixelShaderPtr& ps, constant cc)
{
	Matrix4x4 translation;

	Matrix4x4 temp;
	temp.inverse();

	cc.m_time = 0;
	cc.m_world.setIdentity();
	cc.m_world = GraphicsEngine::get()->getCameraSystem()->getCurrentCameraWorld();
	cc.m_world.flipZBasisVector();
	cc.m_world.setTranslation(m_world_cam.getTranslation());

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(ps, m_cb);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ps);

	m_gizmo_icon->draw();
}

void Camera::switchProjectionMode()
{
	m_is_perspective = !m_is_perspective;
}

void Camera::setOrthographicView()
{
	AppWindow* app = AppWindow::get();
	int width = (app->getClientWindowRect().right - app->getClientWindowRect().left);
	int height = (app->getClientWindowRect().bottom - app->getClientWindowRect().top);

	m_proj_cam.setOrthoLH(width/400.0f, height/400.0f, -4.0f, 4.0f);
}

void Camera::setPerspectiveView()
{
	AppWindow* app = AppWindow::get();
	int width = (app->getClientWindowRect().right - app->getClientWindowRect().left);
	int height = (app->getClientWindowRect().bottom - app->getClientWindowRect().top);

	m_proj_cam.setPerspectiveFovLH(m_field_of_view, (float)width / (float)height, m_near_clip_plane, m_far_clip_plane);
}

void Camera::updateQuad()
{
	Vector3D pos = getWorldCameraTranslation();
}

Matrix4x4 Camera::getViewMatrix()
{
	return m_view_cam;
}

Matrix4x4 Camera::getProjection()
{
	if (m_is_perspective) setPerspectiveView();
	else setOrthographicView();

	return m_proj_cam;
}
