#include "AGameObject.h"

AGameObject::AGameObject(String name)
{
	m_name = name;
	m_local_position = Vector3D::zero();
	m_local_rotation = Vector3D::zero();
	m_local_scale = Vector3D::one();

	m_local_matrix.setIdentity();
}

AGameObject::~AGameObject()
{
	for (int i = 0; i < m_component_list.size(); i++) {
		m_component_list[i]->detachOwner();
	}
	m_component_list.clear();
}

void AGameObject::update(float deltaTime)
{
}

void AGameObject::draw(int width, int height)
{
}

void AGameObject::setPosition(float x, float y, float z)
{
	m_local_position = Vector3D(x, y, z);
	
	m_override_matrix = false;
}

void AGameObject::setPosition(Vector3D pos)
{
	m_local_position = pos;
	
	m_override_matrix = false;
}

Vector3D AGameObject::getLocalPosition()
{
	return m_local_position;
}

void AGameObject::setRotation(float x, float y, float z)
{
	m_orientation = {};
	m_orientation.x = x;
	m_orientation.y = y;
	m_orientation.z = z;

	m_override_matrix = false;
}

void AGameObject::setRotation(Vector3D rot)
{
	m_orientation = {};
	m_orientation.x = rot.m_x;
	m_orientation.y = rot.m_y;
	m_orientation.z = rot.m_z;

	m_override_matrix = false;
}

void AGameObject::setRotation(float x, float y, float z, float w)
{
	m_orientation = {};
	m_orientation.x = x;
	m_orientation.y = y;
	m_orientation.z = z;
	m_orientation.w = w;

	m_override_matrix = false;
}

Vector3D AGameObject::getLocalRotation()
{
	//return m_local_rotation;
	return Vector3D(m_orientation.x, m_orientation.y, m_orientation.z);
}

void AGameObject::setScale(float x, float y, float z)
{
	m_local_scale = Vector3D(x, y, z);

	m_override_matrix = false;
}

void AGameObject::setScale(Vector3D scale)
{
	m_local_scale = scale;
	
	m_override_matrix = false;
}

Vector3D AGameObject::getLocalScale()
{
	return m_local_scale;
}

bool AGameObject::isEnabled()
{
	return m_enabled;
}

void AGameObject::setEnabled(bool flag)
{
	m_enabled = flag;
}

AGameObject::String AGameObject::getName()
{
	return m_name;
}

void AGameObject::setName(String name)
{
	m_name = name;
}

void AGameObject::attachComponent(AComponent* component)
{
	m_component_list.push_back(component);

	component->attachOwner(this);
}

void AGameObject::detachComponent(AComponent* component)
{
	int index = -1;
	for (int i = 0; i < m_component_list.size(); i++) {
		if (m_component_list[i] == component) {
			index = i;
			break;
		}
	}

	if (index != -1) m_component_list.erase(m_component_list.begin() + index);
}

AComponent* AGameObject::findComponentByName(String name)
{
	for (int i = 0; i < m_component_list.size(); i++)
		if (m_component_list[i]->getName() == name)
			return  m_component_list[i];

	return NULL;
}

AComponent* AGameObject::findComponentOfType(AComponent::ComponentType type, String name)
{
	for (int i = 0; i < m_component_list.size(); i++)
		if (m_component_list[i]->getName() == name && m_component_list[i]->getType() == type)
			return m_component_list[i];

	return NULL;
}

AGameObject::ComponentList AGameObject::getComponentsOfType(AComponent::ComponentType type)
{
	ComponentList foundList;
	for (int i = 0; i < m_component_list.size(); i++)
		if (m_component_list[i]->getType() == type)
			foundList.push_back(m_component_list[i]);

	return foundList;
}

AGameObject::ComponentList AGameObject::getComponentsOfTypeRecursive(AComponent::ComponentType type)
{
	ComponentList foundList;
	for (int i = 0; i < m_component_list.size(); i++)
		if (m_component_list[i]->getType() == type)
			foundList.push_back(m_component_list[i]);

	return foundList;
}

void AGameObject::updateLocalMatrix()
{
	// prepare matrices
	Matrix4x4 allMatrix; allMatrix.setIdentity();
	Matrix4x4 translationMatrix; translationMatrix.setIdentity(); translationMatrix.setTranslation(getLocalPosition());
	Matrix4x4 scaleMatrix; scaleMatrix.setIdentity(); scaleMatrix.setScale(getLocalScale());
	Vector3D rotation = getLocalRotation();
	Matrix4x4 zMatrix; zMatrix.setIdentity(); zMatrix.setRotationZ(rotation.m_z);
	Matrix4x4 xMatrix; xMatrix.setIdentity(); xMatrix.setRotationX(rotation.m_x);
	Matrix4x4 yMatrix; yMatrix.setIdentity(); yMatrix.setRotationY(rotation.m_y);

	// multiply scale to rotation, then product to translation
	Matrix4x4 rotMatrix; rotMatrix.setIdentity();
	yMatrix *= zMatrix;
	xMatrix *= yMatrix;
	rotMatrix *= xMatrix;

	scaleMatrix *= rotMatrix;
	allMatrix *= scaleMatrix;
	allMatrix *= translationMatrix;
	m_local_matrix = allMatrix;
}

void AGameObject::setLocalMatrix(float matrix[16])
{
	float matrix4x4[4][4];

	matrix4x4[0][0] = matrix[0];
	matrix4x4[0][1] = matrix[1];
	matrix4x4[0][2] = matrix[2];
	matrix4x4[0][3] = matrix[3];

	matrix4x4[1][0] = matrix[4];
	matrix4x4[1][1] = matrix[5];
	matrix4x4[1][2] = matrix[6];
	matrix4x4[1][3] = matrix[7];

	matrix4x4[2][0] = matrix[8];
	matrix4x4[2][1] = matrix[9];
	matrix4x4[2][2] = matrix[10];
	matrix4x4[2][3] = matrix[11];

	matrix4x4[3][0] = matrix[12];
	matrix4x4[3][1] = matrix[13];
	matrix4x4[3][2] = matrix[14];
	matrix4x4[3][3] = matrix[15];

	Matrix4x4 newMatrix; newMatrix.setIdentity(); newMatrix.setMatrix(matrix4x4);
	Matrix4x4 scaleMatrix; scaleMatrix.setIdentity(); scaleMatrix.setScale(m_local_scale);
	Matrix4x4 transMatrix; transMatrix.setIdentity(); transMatrix.setTranslation(m_local_position);

	m_local_matrix = scaleMatrix.multiplyTo(transMatrix.multiplyTo(newMatrix));

	m_override_matrix = true;
}

float* AGameObject::getRawMatrix()
{
	float* matrix4x4 = m_local_matrix.getMatrix();
	return matrix4x4;
}

float* AGameObject::getPhysicsLocalMatrix()
{
	// prepare matrices
	Matrix4x4 allMatrix; allMatrix.setIdentity();
	Matrix4x4 translationMatrix; translationMatrix.setIdentity(); translationMatrix.setTranslation(getLocalPosition());
	Matrix4x4 scaleMatrix; scaleMatrix.setIdentity(); scaleMatrix.setScale(Vector3D::one());
	Vector3D rotation = getLocalRotation();
	Matrix4x4 zMatrix; zMatrix.setIdentity(); zMatrix.setRotationZ(rotation.m_z);
	Matrix4x4 xMatrix; xMatrix.setIdentity(); xMatrix.setRotationX(rotation.m_x);
	Matrix4x4 yMatrix; yMatrix.setIdentity(); yMatrix.setRotationY(rotation.m_y);

	// multiply scale to rotation, then product to translation
	Matrix4x4 rotMatrix; rotMatrix.setIdentity();
	yMatrix *= zMatrix;
	xMatrix *= yMatrix;
	rotMatrix *= xMatrix;

	scaleMatrix *= rotMatrix;
	allMatrix *= scaleMatrix;
	allMatrix *= translationMatrix;

	return allMatrix.getMatrix();
}