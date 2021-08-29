#pragma once

#include <string>
#include <vector>
#include <reactphysics3d/reactphysics3d.h>

#include "Vector3D.h"
#include "Matrix4x4.h"
#include "ConstantData.h"
#include "Prerequisites.h"
#include "AComponent.h"

class VertexShader;
class PixelShader;

class AGameObject
{
public:
	struct AQuaternion {
		float w = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	typedef std::string String;
	typedef std::vector<AComponent*> ComponentList;

	AGameObject(String name);
	~AGameObject();

	virtual void update(float deltaTime);
	virtual void draw(int width, int height);

	void setPosition(float x, float y, float z);
	void setPosition(Vector3D pos);
	Vector3D getLocalPosition();

	void setRotation(float x, float y, float z);
	void setRotation(Vector3D rot);
	void setRotation(float x, float y, float z, float w);
	Vector3D getLocalRotation();

	void setScale(float x, float y, float z);
	void setScale(Vector3D scale);
	Vector3D getLocalScale();

	bool isEnabled();
	void setEnabled(bool flag);

	String getName();
	void setName(String name);

	void attachComponent(AComponent* component);
	void detachComponent(AComponent* component);

	AComponent* findComponentByName(String name);
	AComponent* findComponentOfType(AComponent::ComponentType type, String name);
	ComponentList getComponentsOfType(AComponent::ComponentType type);
	ComponentList getComponentsOfTypeRecursive(AComponent::ComponentType type);

	void updateLocalMatrix(); //updates local matrix based from latest position, rotation, and scale.
	void setLocalMatrix(float matrix[16]);
	Matrix4x4 getLocalMatrix();
	float* getRawMatrix();
	float* getPhysicsLocalMatrix(); //scale is set to 1.0

protected:
	String m_name;
	Vector3D m_local_position;
	Vector3D m_local_rotation;
	AQuaternion m_orientation;
	Vector3D m_local_scale;
	
	Matrix4x4 m_local_matrix;

	ComponentList m_component_list;

	bool m_override_matrix = false;

private:
	bool m_enabled = true;
};