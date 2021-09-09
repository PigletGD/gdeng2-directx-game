#pragma once
#include <iostream>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "AGameObject.h"

class EditorAction
{
public:
	EditorAction(AGameObject* gameObject);
	~EditorAction();

	AGameObject::String getOwnerName();
	Vector3D getStorePos();
	Vector3D getStoredScale();
	AGameObject::AQuaternion getStoredOrientation();
	Matrix4x4 getStoredMatrix();

private:
	AGameObject::String m_object_name;
	Vector3D m_local_position;
	Vector3D m_local_scale;
	AGameObject::AQuaternion m_orientation;
	Matrix4x4 m_local_matrix;
};