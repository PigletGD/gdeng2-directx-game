#include "AGameObject.h"

AGameObject::AGameObject(std::string name)
{
	m_name = name;
	m_local_position = Vector3D::zero();
	m_local_rotation = Vector3D::zero();
	m_local_scale = Vector3D::one();
}

AGameObject::~AGameObject()
{
}

void AGameObject::update(float deltaTime)
{
}

void AGameObject::draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader, constant cc)
{
}

void AGameObject::setPosition(float x, float y, float z)
{
	m_local_position = Vector3D(x, y, z);
}

void AGameObject::setPosition(Vector3D pos)
{
	m_local_position = pos;
}

Vector3D AGameObject::getLocalPosition()
{
	return m_local_position;
}

void AGameObject::setRotation(float x, float y, float z)
{
	m_local_rotation = Vector3D(x, y, z);
}

void AGameObject::setRotation(Vector3D rot)
{
	m_local_rotation = rot;
}

Vector3D AGameObject::getLocalRotation()
{
	return m_local_rotation;
}

void AGameObject::setScale(float x, float y, float z)
{
	m_local_scale = Vector3D(x, y, z);
}

void AGameObject::setScale(Vector3D scale)
{
	m_local_scale = scale;
}

Vector3D AGameObject::getLocalScale()
{
	return m_local_scale;
}