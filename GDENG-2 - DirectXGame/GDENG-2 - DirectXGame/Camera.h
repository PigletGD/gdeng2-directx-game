#pragma once
#include "Quad.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "ConstantData.h"
#include "Prerequisites.h"

class Camera
{
public:
	Camera();
	~Camera();

	void createBuffers(void* shader_byte_code, UINT size_byte_shader);

	void setWorldCameraMatrix(Matrix4x4 world_cam);
	Matrix4x4 getWorldCameraMatrix();
	void setWorldCameraTranslation(Vector3D new_pos);
	Vector3D getWorldCameraTranslation();

	float getXRot();
	float getYRot();
	void updateRotation(float delta_rot_x, float delta_rot_y);

	void drawGizmoIcon(const VertexShaderPtr& vs, const PixelShaderPtr& ps, constant cc);

	void switchProjectionMode();
	void setOrthographicView();
	void setPerspectiveView();

	void updateQuad();

	Matrix4x4 getProjection();

private:
	Matrix4x4 m_world_cam;
	Matrix4x4 m_proj_cam;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	bool m_is_perspective = true;

	float m_field_of_view = 1.57f;
	float m_near_clip_plane = 0.1f;
	float m_far_clip_plane = 100.0f;

	Quad* m_gizmo_icon;

	ConstantBufferPtr m_cb;
};