#include "AGameObject.h"
#include <algorithm>
#include <vector>
#include "EditorAction.h"

#include "PhysicsComponent.h"
AGameObject::AGameObject(String name, PrimitiveType type)
{
	m_name = name;
	m_object_type = type;

	m_local_position = Vector3D::zero();
	//m_local_rotation = Vector3D::zero();
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

AGameObject::PrimitiveType AGameObject::getObjectType()
{
	return m_object_type;
}

void AGameObject::update(float deltaTime)
{
}

void AGameObject::draw(int width, int height)
{
}

void AGameObject::setPosition(float x, float y, float z)
{
	if(this->m_child_list.empty())
	{
		m_local_position = Vector3D(x, y, z);
		m_override_matrix = false;
		return;
	}

	Vector3D prevPosition = this->m_local_position;
	m_local_position = Vector3D(x, y, z);
	m_override_matrix = false;

	for(AGameObject* child : this->m_child_list)
	{
		Vector3D displacement = child->getLocalPosition() - prevPosition;
		child->setPosition(Vector3D(x, y, z) + displacement);
	}
	

}

void AGameObject::setPosition(Vector3D pos)
{
	if(this->m_child_list.empty())
	{
		m_local_position = pos;
		m_override_matrix = false;
		return;
	}
	Vector3D prevPosition = this->m_local_position;
	
	m_local_position = pos;
	m_override_matrix = false;

	
	for(AGameObject* child : this->m_child_list)
	{
		Vector3D displacement = child->getLocalPosition() - prevPosition;
		child->setPosition(pos + displacement);
	}

	/*ComponentList compList = this->getComponentsOfType(AComponent::Physics);
	PhysicsComponent* pcomp = nullptr;
	if (!compList.empty())
	{
		for (int i = 0; i < compList.size(); i++)
		{
			if (compList[i]->getType() == AComponent::Physics)
			{
				pcomp = dynamic_cast<PhysicsComponent*>(compList[i]);
				break;
			}
		}
		if (pcomp != nullptr)
		{
			//update the rigidbody transforms
			Transform t;
			t.setPosition(Vector3(m_local_position.m_x, m_local_position.m_y, m_local_position.m_z));
			//t.setOrientation(Quaternion(m_orientation.m_x, m_orientation.m_y, m_orientation.m_z, m_orientation.m_w));
			pcomp->getRigidBody()->setTransform(t);
			//pcomp->perform(0);
		}
	}*/

}

Vector3D AGameObject::getLocalPosition()
{
	return m_local_position;
}

void AGameObject::setRotation(float x, float y, float z)
{
	//for the camera orientation stuff
	m_orientation = {};
	m_orientation.m_x = x;
	m_orientation.m_y = y;
	m_orientation.m_z = z;
	m_orientation.m_w = 1;
	m_override_matrix = false;

}

void AGameObject::setRotation(Vector3D rot)
{
	//this is used when changing values from the inspector window
	std::cout << "BRUH\n";
	std::cout << "Name: "<<this->getName() <<std::endl;
	if(this->m_child_list.empty())
	{
		std::cout << "Empty childlist\n";
		m_orientation = {};
		m_orientation.m_x = rot.m_x;
		m_orientation.m_y = rot.m_y;
		m_orientation.m_z = rot.m_z;
		m_orientation.m_w = 1; //identity quaternion
		
		m_override_matrix = false;
		return;
	}

	std::cout << "childlist has value\n";
	Vector3D oldRot = this->getLocalRotation();
	m_orientation = {};
	m_orientation.m_x = rot.m_x;
	m_orientation.m_y = rot.m_y;
	m_orientation.m_z = rot.m_z;
	m_orientation.m_w = 1;
	m_override_matrix = false;
	
	Vector3D newRot = this->getLocalRotation();
	Vector3D rotDiff = newRot - oldRot;

	
	
	Vector3D savedPos = this->m_local_position;
	setPosition(Vector3D(0, 0, 0));

	for (AGameObject* child : this->m_child_list) {
		

		//Vector3D initialPosition;
		Vector3D initialPosition = child->getLocalPosition();
		Vector3D newPosition = Vector3D::one();
		
		if (rotDiff.m_x != 0) {
			AQuaternion xRot = AQuaternion(Vector3D(1, 0,0), rotDiff.m_x);

			//initialPosition = child->getLocalPosition();
			newPosition = AQuaternion::Rotate(&initialPosition, xRot);
		}
	
		else if (rotDiff.m_y != 0) {
			AQuaternion yRot = AQuaternion(Vector3D(0, 1, 0), rotDiff.m_y);
			//initialPosition = child->getLocalPosition();
			newPosition = AQuaternion::Rotate(&initialPosition, yRot);
		}
		else if (rotDiff.m_z != 0) {
			AQuaternion zRot = AQuaternion(Vector3D(0, 0, 1), rotDiff.m_z);
			//initialPosition = child->getLocalPosition();
			newPosition = AQuaternion::Rotate(&initialPosition, zRot);
		}
		
		
	
		child->setRotation(rotDiff + child->getLocalRotation());
		child->setPosition(newPosition);
		
	}

	setPosition(savedPos);
	
	
}

void AGameObject::setRotation(float x, float y, float z, float w)
{
	
	if(this->m_child_list.empty())
	{
		m_orientation = {};
		m_orientation.m_x = x;
		m_orientation.m_y = y;
		m_orientation.m_z = z;
		m_orientation.m_w = w;
		m_override_matrix = false;
		return;
	}

	Vector3D oldRot = this->getLocalRotation();
	m_orientation = {};
	m_orientation.m_x = x;
	m_orientation.m_y = y;
	m_orientation.m_z = z;
	m_orientation.m_w = w;
	m_override_matrix = false;

	Vector3D newRotation = this->getLocalRotation();
	Vector3D rotDiff = newRotation - oldRot;
	
	

	for (AGameObject* child : this->m_child_list) {
		
		Vector3D savedPos = this->m_local_position;
		setPosition(Vector3D(0, 0, 0));
		
		Vector3D initialPosition;
		Vector3D newPosition;
		
		if (rotDiff.m_x != 0) {
			AQuaternion xRot = AQuaternion(Vector3D(1, m_local_position.m_y, m_local_position.m_z), rotDiff.m_x);
			
			initialPosition = child->getLocalPosition();
			newPosition = AQuaternion::Rotate(&initialPosition, xRot);
		}

		else if (rotDiff.m_y != 0) {
			AQuaternion yRot = AQuaternion(Vector3D(m_local_position.m_x, 1, m_local_position.m_z), rotDiff.m_y);
			initialPosition = child->getLocalPosition();
			newPosition = AQuaternion::Rotate(&initialPosition, yRot);
		}

		else if (rotDiff.m_z != 0) {
			AQuaternion zRot = AQuaternion(Vector3D(m_local_position.m_x, m_local_position.m_y, 1), rotDiff.m_z);
			initialPosition = child->getLocalPosition();
			newPosition = AQuaternion::Rotate(&initialPosition, zRot);
		}
		
		child->setRotation(rotDiff + child->getLocalRotation());
		child->setPosition(newPosition);
		
	
		setPosition(savedPos);
	}

	
}

Vector3D AGameObject::getLocalRotation()
{
	//return m_local_rotation;
	return Vector3D(m_orientation.m_x, m_orientation.m_y, m_orientation.m_z);
}

void AGameObject::setScale(float x, float y, float z)
{
	if(this->m_child_list.empty())
	{
		m_local_scale = Vector3D(x, y, z);
		m_override_matrix = false;
		return;
	}

	Vector3D oldScale = this->m_local_scale;
	m_local_scale = Vector3D(x, y, z);
	m_override_matrix = false;

	if (oldScale.m_x != m_local_scale.m_x) {
		//For all childs
		for (AGameObject* child : m_child_list) {
			Vector3D childScale = Vector3D((m_local_scale.m_x / oldScale.m_x) * child->getLocalScale().m_x, child->getLocalScale().m_y, child->getLocalScale().m_z);
			child->setScale(childScale);
		}
	}

	else if (oldScale.m_x != m_local_scale.m_x) {
		for (AGameObject* child : m_child_list) {
			Vector3D childScale = Vector3D(child->getLocalScale().m_x, child->getLocalScale().m_y * (m_local_scale.m_y / oldScale.m_y), child->getLocalScale().m_z);
			child->setScale(childScale);
		}
	}

	else if (oldScale.m_x != m_local_scale.m_x) {
		for (AGameObject* child : m_child_list) {
			Vector3D childScale = Vector3D(child->getLocalScale().m_x, child->getLocalScale().m_y, (m_local_scale.m_z / oldScale.m_z) * child->getLocalScale().m_z);
			child->setScale(childScale);
		}
	}
	
}

void AGameObject::setScale(Vector3D scale)
{
	if(this->m_child_list.empty())
	{
		m_local_scale = scale;
		m_override_matrix = false;
		return;
	}
	Vector3D oldScale = this->m_local_scale;
	m_local_scale = scale;
	m_override_matrix = false;
	if (oldScale.m_x != m_local_scale.m_x) {
		//For all childs
		for (AGameObject* child : m_child_list) {
			Vector3D childScale = Vector3D((m_local_scale.m_x / oldScale.m_x) * child->getLocalScale().m_x, child->getLocalScale().m_y, child->getLocalScale().m_z);
			child->setScale(childScale);
		}
	}

	else if (oldScale.m_x != m_local_scale.m_x) {
		for (AGameObject* child : m_child_list) {
			Vector3D childScale = Vector3D(child->getLocalScale().m_x, child->getLocalScale().m_y * (m_local_scale.m_y / oldScale.m_y), child->getLocalScale().m_z);
			child->setScale(childScale);
		}
	}

	else if (oldScale.m_x != m_local_scale.m_x) {
		for (AGameObject* child : m_child_list) {
			Vector3D childScale = Vector3D(child->getLocalScale().m_x, child->getLocalScale().m_y, (m_local_scale.m_z / oldScale.m_z) * child->getLocalScale().m_z);
			child->setScale(childScale);
		}
	}

	
	
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

AGameObject::ComponentList AGameObject::getAllObjectComponents()
{
	return this->m_component_list;
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

Matrix4x4 AGameObject::getLocalMatrix()
{
	return m_local_matrix;
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

void AGameObject::updateTexture(TexturePtr newTex)
{
}

void AGameObject::saveEditState()
{
	if (m_last_edit_state == NULL)
		m_last_edit_state = new EditorAction(this);
}

void AGameObject::restoreEditState()
{
	if (m_last_edit_state != NULL) {
		m_local_position = m_last_edit_state->getStorePos();
		m_local_scale = m_last_edit_state->getStoredScale();
		m_orientation = m_last_edit_state->getStoredOrientation();
		m_local_matrix = m_last_edit_state->getStoredMatrix();

		m_last_edit_state = NULL;

		/*
		ComponentList compList = this->getComponentsOfType(AComponent::Physics);
		PhysicsComponent* pcomp = nullptr;
		if(!compList.empty())
		{
			for(int i = 0 ; i < compList.size(); i++)
			{
				if(compList[i]->getType() == AComponent::Physics)
				{
					pcomp = dynamic_cast<PhysicsComponent*>(compList[i]);
					break;
				}
			}
			if(pcomp != nullptr)
			{
				//update the rigidbody transforms
				Transform t;
				t.setPosition(Vector3(m_local_position.m_x, m_local_position.m_y, m_local_position.m_z));
				t.setOrientation(Quaternion(m_orientation.m_x, m_orientation.m_y, m_orientation.m_z, m_orientation.m_w));
				pcomp->getRigidBody()->setTransform(t);
			}
		}
		*/
	}
	else std::cout << "Edit state is null. Cannot restore. \n";
}

void AGameObject::addChild(AGameObject* childObject)
{
	if(this->isInList(childObject))
	{
		return;
	}
	this->m_child_list.push_back(childObject);
}

void AGameObject::removeChild(AGameObject* childObject)
{
	if(this->isInList(childObject))
	{
		return;
	}

	this->m_child_list.erase(this->m_child_list.begin() + this->findChildIndexInList(childObject));
	
}

bool AGameObject::isInList(AGameObject* object)
{
	int index = -1;
	for(int i = 0; i < this->m_child_list.size() ; i++)
	{
		if(this->m_child_list[i] == object)
		{
			index = i;
			break;
		}
	}

	if(index == -1)
	{
		return false;
	}
	return true;
}

int AGameObject::findChildIndexInList(AGameObject* childObject)
{
	int index = -1;
	for(int i = 0; i < this->m_child_list.size() ; i++)
	{
		if(this->m_child_list[i] == childObject)
		{
			index = i;
			break;
		}
	}

	return index;
}

void AGameObject::setSelected(bool isselected)
{
	this->m_selected = isselected;
}

bool AGameObject::isSelected()
{
	return this->m_selected;
}


void AGameObject::awake()
{
}
