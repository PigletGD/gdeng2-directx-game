#pragma once
#include <iostream>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "AGameObject.h"
#include "Quaternion.h"
#include "Vector4D.h"

class EditorAction
{
public:
	EditorAction(AGameObject* gameObject);
	~EditorAction();

	AGameObject::String getOwnerName();
	Vector3D getStorePos();
	Vector3D getStoredScale();
	AQuaternion getStoredOrientation();
	Matrix4x4 getStoredMatrix();

private:
	AGameObject::String m_object_name;
	Vector3D m_local_position;
	Vector3D m_local_scale;
	AQuaternion m_orientation;
	Matrix4x4 m_local_matrix;
	
};