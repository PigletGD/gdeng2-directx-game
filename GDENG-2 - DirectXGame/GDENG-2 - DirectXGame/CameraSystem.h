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

	Camera* getCurrentCamera();

	void addNewCamera();
	void addNewCamera(Camera* new_camera);
	void removeCurrentCamera();
	void removeCamera(Camera* camera);

	void switchCamera(Camera* camera);
	void switchToPreviousCamera();
	void switchToNextCamera();

	void updateCurrentCamera();
	void updateCurrentCameraWindowSize(float width, float height);

	void setCurrentCamera(Camera* camera);
	void setCurrentToMainViewCamera();

	Matrix4x4 getCurrentCameraWorldMatrix();
	Matrix4x4 getCurrentCameraViewMatrix();
	Matrix4x4 getCurrentCameraProjectionMatrix();

	void initializeGizmoTexture();
	void initializeInitialCamera();
	void drawGizmos(constant cc);

	void setHoverViewportState(bool value);
	void incrementFocusCount();
	void decrementFocusCount();
	void updateInputListener();

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

private:
	std::vector<Camera*> cameraList;
	int m_main_view_camera_index = 0;
	int m_view_camera_index = 0;
	int m_control_camera_index = 0;

	int m_viewport_camera_count = 0;

	int m_window_focus_count = 0;
	bool m_is_listening = false;
	bool m_is_over_viewport = false;
	bool m_lock_set_window_focus = false;

	float m_cam_speed = 3.0f;

	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	bool m_pressed_rotate_cam = false;
	bool m_pressed_add_cam = false;
	bool m_pressed_remove_cam = false;
	bool m_pressed_prev_cam = false;
	bool m_pressed_next_cam = false;
	bool m_pressed_switch_proj = false;

	// Camera System Keyboard Controls
	int m_add_cam_key =		'P';
	int m_remove_cam_key =	'O';
	int m_prev_cam_key =	'Q';
	int m_next_cam_key =	'E';
	int m_switch_proj_key = 'M';

	TexturePtr m_gizmo_icon;

	void* m_shader_byte_code = nullptr;
	size_t m_size_shader = 0;

	VertexShaderPtr m_gizmo_vs;
	PixelShaderPtr m_gizmo_ps;

	Point m_old_mouse_pos;
};