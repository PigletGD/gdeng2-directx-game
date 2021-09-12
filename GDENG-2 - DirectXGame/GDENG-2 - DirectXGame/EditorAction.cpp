#include "EditorAction.h"

EditorAction::EditorAction(AGameObject* game_object)
{
	m_object_name = game_object->getName();
	
	m_local_position = game_object->getLocalPosition();
	
	m_orientation = {};
	Vector3D rotation = game_object->getLocalRotation();
	m_orientation.m_x = rotation.m_x;
	m_orientation.m_y = rotation.m_y;
	m_orientation.m_z = rotation.m_z;

	m_local_scale = game_object->getLocalScale();

	m_local_matrix = game_object->getLocalMatrix();
}

EditorAction::~EditorAction()
{

}

AGameObject::String EditorAction::getOwnerName()
{
	return m_object_name;
}

Vector3D EditorAction::getStorePos()
{
	return m_local_position;
}

Vector3D EditorAction::getStoredScale()
{
	return m_local_scale;
}

AQuaternion EditorAction::getStoredOrientation()
{
	return m_orientation;
}

Matrix4x4 EditorAction::getStoredMatrix()
{
	return m_local_matrix;
}