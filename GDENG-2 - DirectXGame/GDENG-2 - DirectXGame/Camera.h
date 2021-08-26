#pragma once
#include "AGameObject.h"
#include "Quad.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "ConstantData.h"
#include "Prerequisites.h"

class Camera : public AGameObject
{
public:
	Camera(float width, float height);
	~Camera();

	void updatePosition(float speed, float forward, float rightward);
	void updateRotation(float delta_rot_x, float delta_rot_y);

	void updateWorldAndViewMatrix();
	void updateWindowSize(float width, float height);

	void setToPerspectiveMode(bool value);
	void switchProjectionMode();
	void setOrthographicView();
	void setPerspectiveView();

	void setToNormalViewMode();
	void setToTopDownViewMode();
	void setToFrontViewMode();
	void setToRighViewMode();

	Matrix4x4 getWorldMatrix();
	Matrix4x4 getViewMatrix();
	Matrix4x4 getProjectionMatrix();

	void createBuffersAndShaders();
	void drawGizmoIcon(constant cc);

private:
	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;

	Vector3D m_retained_pos;
	Vector3D m_retained_rot;

	float m_window_width;
	float m_window_height;

	bool m_is_perspective = true;

	float m_field_of_view = 1.57f;
	float m_near_clip_plane = 0.1f;
	float m_far_clip_plane = 100.0f;

	Quad* m_gizmo_icon;

	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	ConstantBufferPtr m_cb;

	bool m_is_active;
	bool m_is_in_direction_view = false;

	friend class CameraSystem;
};