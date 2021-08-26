#pragma once

#include "Matrix4x4.h"

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_time;
	float m_lerp_speed;
	float isLit;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
};