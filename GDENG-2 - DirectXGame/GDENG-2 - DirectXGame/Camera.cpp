#include "Camera.h"
#include "AppWindow.h"

Camera::Camera()
{
	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	m_gizmo_icon = new Quad({Vector3D(-0.5f, 0.5f,-1.0f), Vector2D(0, 1)},
							{Vector3D( 0.5f, 0.5f,-1.0f), Vector2D(1, 1)},
							{Vector3D( 0.5f,-0.5f,-1.0f), Vector2D(1, 0)},
							{Vector3D(-0.5f,-0.5f,-1.0f), Vector2D(0, 0)});
}

Camera::~Camera()
{
}

void Camera::createBuffers(void* shader_byte_code, UINT size_byte_shader)
{
	m_gizmo_icon->createBuffers(shader_byte_code, size_byte_shader);
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

void Camera::updateRotation(float rot_x, float rot_y)
{
	m_rot_x += rot_x;
	m_rot_y += rot_y;
}

void Camera::drawGizmoIcon()
{
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

Matrix4x4 Camera::getProjection()
{
	if (m_is_perspective) setPerspectiveView();
	else setOrthographicView();

	return m_proj_cam;
}
