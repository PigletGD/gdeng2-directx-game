#include "PhysicsComponent.h"

#include <iostream>

#include "ComponentSystem.h"
#include "PhysicsSystem.h"
#include "AGameObject.h"
#include "Vector3D.h"

PhysicsComponent::PhysicsComponent(String name, AGameObject* owner) :
    AComponent(name, ComponentType::Physics, owner)
{
	//whenever a new physics component is initialized. Register to physics system
	ComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
	PhysicsCommon* physicsCommon = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
	PhysicsWorld* physicsWorld = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

	// Create a rigid body in the world
	Vector3D scale = m_owner->getLocalScale();
	Transform transform; transform.setFromOpenGL(m_owner->getPhysicsLocalMatrix());
	BoxShape* boxShape = physicsCommon->createBoxShape(Vector3(scale.m_x / 2, scale.m_y / 2, scale.m_z / 2)); //half extent

	m_rigid_body = physicsWorld->createRigidBody(transform);
	m_rigid_body->addCollider(boxShape, transform);
	m_rigid_body->updateMassPropertiesFromColliders();
	m_rigid_body->setMass(m_mass);
	m_rigid_body->setType(BodyType::DYNAMIC);

	float matrix[16];
	transform = m_rigid_body->getTransform();
	transform.getOpenGLMatrix(matrix);

	m_owner->setLocalMatrix(matrix);
}

PhysicsComponent::~PhysicsComponent()
{
	ComponentSystem::getInstance()->getPhysicsSystem()->unregisterComponent(this);
}

void PhysicsComponent::perform(float delta_time)
{
	float matrix[16];
    const Transform transform = m_rigid_body->getTransform();

	float x = transform.getPosition().x;
	float y = transform.getPosition().y;
	float z = transform.getPosition().z;

    transform.getOpenGLMatrix(matrix);

    m_owner->setLocalMatrix(matrix);
}

RigidBody* PhysicsComponent::getRigidBody()
{
    return m_rigid_body;
}
