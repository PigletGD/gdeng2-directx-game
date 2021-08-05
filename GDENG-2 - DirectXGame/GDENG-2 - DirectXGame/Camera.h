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
	Camera();
	~Camera();

	void updatePosition(float speed, float forward, float rightward);
	void updateRotation(float delta_rot_x, float delta_rot_y);

	void updateWorldAndViewMatrix();

	void switchProjectionMode();
	void setOrthographicView();
	void setPerspectiveView();

	Matrix4x4 getWorldMatrix();
	Matrix4x4 getViewMatrix();
	Matrix4x4 getProjectionMatrix();

	void createBuffersAndShaders();
	void drawGizmoIcon(constant cc);

private:
	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;

	bool m_is_perspective = true;

	float m_field_of_view = 1.57f;
	float m_near_clip_plane = 0.1f;
	float m_far_clip_plane = 100.0f;

	Quad* m_gizmo_icon;

	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	ConstantBufferPtr m_cb;
};