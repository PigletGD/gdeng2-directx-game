#pragma once

#include "InputListener.h"
#include "Camera.h"
#include "Matrix4x4.h"
#include "ConstantData.h"
#include "Prerequisites.h"

#include <vector>

class CameraSystem : public InputListener
{
public:
	CameraSystem();
	~CameraSystem();

	void initializeGizmoTexture();
	void createCameraBuffers(void* shader_byte_code, UINT size_byte_shader);
	void setCameraShaders(const VertexShaderPtr& vs, const PixelShaderPtr& ps);

	void switchToPreviousCamera();
	void switchToNextCamera();

	void updateCurrentCamera();
	
	Matrix4x4 getCurrentCameraWorld();
	Matrix4x4 getCurrentCameraView();
	Matrix4x4 getCurrentCameraProjection();

	void drawGizmos(const VertexShaderPtr& vs, const PixelShaderPtr& ps, constant cc);

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

private:
	std::vector<Camera*> cameraList;
	int m_camera_index = 0;

	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	bool m_pressed_q = false;
	bool m_pressed_e = false;
	bool m_pressed_m = false;

	TexturePtr m_gizmo_icon;

	VertexShaderPtr m_gizmo_vs;
	PixelShaderPtr m_gizmo_ps;
};