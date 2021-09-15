#pragma once

#include <string>
#include <vector>
#include <reactphysics3d/reactphysics3d.h>

#include "Vector3D.h"
#include "Matrix4x4.h"
#include "ConstantData.h"
#include "Prerequisites.h"
#include "AComponent.h"
#include "Quaternion.h"

class VertexShader;
class PixelShader;
class EditorAction;
class HierarchyScreen;

class AGameObject
{
public:
	enum PrimitiveType {
		NONE,
		CAMERA,
		TEXTURED_CUBE,
		CUBE,
		PLANE,
		SPHERE,
		CYLINDER,
		PHYSICS_CUBE,
		PHYSICS_CUBE_BATCH,
		PHYSICS_PLANE,
		MESH
	};

	/*
	struct AQuaternion {
		float w = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};*/

	typedef std::string String;
	typedef std::vector<AComponent*> ComponentList;
	typedef std::vector<AGameObject*> ChildList;

	AGameObject(String name, PrimitiveType type);
	virtual ~AGameObject();

	PrimitiveType getObjectType();

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

	virtual void saveEditState();
	virtual void restoreEditState();

	void addChild(AGameObject* childObject);
	//void addParent(AGameObject* parentObject);
	void removeChild(AGameObject* childObject);
	bool isInList(AGameObject* object);
	int findChildIndexInList(AGameObject* childObject);
	void setSelected(bool isselected);
	bool isSelected();

protected:
	String m_name;
	Vector3D m_local_position;
	//Vector3D m_local_rotation;
	//AQuaternion m_orientation;
	AQuaternion m_orientation;
	Vector3D m_local_scale;
	
	Matrix4x4 m_local_matrix;

	PrimitiveType m_object_type;

	ComponentList m_component_list;
	ChildList m_child_list;

	bool m_override_matrix = false;

	virtual void awake();
	bool m_selected = false;

private:
	bool m_enabled = true;

	EditorAction* m_last_edit_state = NULL;
	friend class HierarchyScreen;
};