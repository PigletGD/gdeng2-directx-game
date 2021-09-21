#include "PhysicsComponent.h"

#include <iostream>

#include "ComponentSystem.h"
#include "PhysicsSystem.h"
#include "AGameObject.h"
#include "Vector3D.h"

#include "Sphere.h"
#include "Capsule.h"

PhysicsComponent::PhysicsComponent(String name, AGameObject* owner) :
    AComponent(name, ComponentType::Physics, owner)
{
	if(owner->getObjectType() == AGameObject::CUBE || owner->getObjectType() == AGameObject::TEXTUREDCUBE || owner->getObjectType() == AGameObject::PHYSICSCUBE || owner->getObjectType() == AGameObject::PHYSICSCUBEBATCH)
	{
		//whenever a new physics component is initialized. Register to physics system
		ComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
		PhysicsCommon* physicsCommon = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
		PhysicsWorld* physicsWorld = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

		// Create a rigid body in the world
		Vector3D position = m_owner->getLocalPosition();
		Vector3D rotation = m_owner->getLocalRotation();
		Vector3D scale = m_owner->getLocalScale();

		reactphysics3d::Vector3 react_pos(position.m_x, position.m_y, position.m_z);
		reactphysics3d::Quaternion react_rot = reactphysics3d::Quaternion::fromEulerAngles(rotation.m_x, rotation.m_y, rotation.m_z);

		//Transform transform; transform.setFromOpenGL(m_owner->getPhysicsLocalMatrix());
		reactphysics3d::Transform transform = reactphysics3d::Transform(react_pos, react_rot);
		BoxShape* boxShape = physicsCommon->createBoxShape(Vector3(scale.m_x / 2, scale.m_y / 2, scale.m_z / 2)); //half extent

		m_rigid_body = physicsWorld->createRigidBody(transform);
		m_rigid_body->addCollider(boxShape, reactphysics3d::Transform::identity());
		m_rigid_body->updateMassPropertiesFromColliders();
		m_rigid_body->setMass(m_mass);
		m_rigid_body->setType(BodyType::DYNAMIC);

		////whenever a new physics component is initialized. Register to physics system
		//ComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
		//PhysicsCommon* physicsCommon = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
		//PhysicsWorld* physicsWorld = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

		//// Create a rigid body in the world
		//Vector3D scale = m_owner->getLocalScale();
		//Transform transform; transform.setFromOpenGL(m_owner->getPhysicsLocalMatrix());
		//BoxShape* boxShape = physicsCommon->createBoxShape(Vector3(scale.m_x / 2, scale.m_y / 2, scale.m_z / 2)); //half extent

		//m_rigid_body = physicsWorld->createRigidBody(transform);
		//m_rigid_body->addCollider(boxShape, transform);
		//m_rigid_body->updateMassPropertiesFromColliders();
		//m_rigid_body->setMass(m_mass);
		//m_rigid_body->setType(BodyType::DYNAMIC);

		//float matrix[16];
		//transform = m_rigid_body->getTransform();
		//transform.getOpenGLMatrix(matrix);

		//m_owner->setLocalMatrix(matrix);
	}
	
	else if(owner->getObjectType() == AGameObject::SPHERE)
	{
		//whenever a new physics component is initialized. Register to physics system
		ComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
		PhysicsCommon* physicsCommon = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
		PhysicsWorld* physicsWorld = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

		// Create a rigid body in the world
		Vector3D position = m_owner->getLocalPosition();
		Vector3D rotation = m_owner->getLocalRotation();
		Vector3D scale = m_owner->getLocalScale();

		reactphysics3d::Vector3 react_pos(position.m_x, position.m_y, position.m_z);
		reactphysics3d::Quaternion react_rot = reactphysics3d::Quaternion::fromEulerAngles(rotation.m_x, rotation.m_y, rotation.m_z);

		//Transform transform; transform.setFromOpenGL(m_owner->getPhysicsLocalMatrix());
		reactphysics3d::Transform transform = reactphysics3d::Transform(react_pos, react_rot);
		Sphere* sphere = dynamic_cast<Sphere*>(owner);
		SphereShape* sphereShape = physicsCommon->createSphereShape(sphere->radius);

		m_rigid_body = physicsWorld->createRigidBody(transform);
		m_rigid_body->addCollider(sphereShape, reactphysics3d::Transform::identity());
		m_rigid_body->updateMassPropertiesFromColliders();
		m_rigid_body->setMass(m_mass);
		m_rigid_body->setType(BodyType::DYNAMIC);

		////whenever a new physics component is initialized. Register to physics system
		//ComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
		//PhysicsCommon* physicsCommon = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
		//PhysicsWorld* physicsWorld = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

		//// Create a rigid body in the world
		//Vector3D scale = m_owner->getLocalScale();
		//Transform transform; transform.setFromOpenGL(m_owner->getPhysicsLocalMatrix());
		//

		//Sphere* sphere = dynamic_cast<Sphere*>(owner);
		//SphereShape* boxShape = physicsCommon->createSphereShape(sphere->radius);
		//
		//m_rigid_body = physicsWorld->createRigidBody(transform);
		//m_rigid_body->addCollider(boxShape, transform);
		//m_rigid_body->updateMassPropertiesFromColliders();
		//m_rigid_body->setMass(m_mass);
		//m_rigid_body->setType(BodyType::DYNAMIC);

		//float matrix[16];
		//transform = m_rigid_body->getTransform();
		//transform.getOpenGLMatrix(matrix);

		//m_owner->setLocalMatrix(matrix);
	}

	else if(owner->getObjectType() == AGameObject::CAPSULE)
	{
		//whenever a new physics component is initialized. Register to physics system
		ComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
		PhysicsCommon* physicsCommon = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
		PhysicsWorld* physicsWorld = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

		// Create a rigid body in the world
		Vector3D position = m_owner->getLocalPosition();
		Vector3D rotation = m_owner->getLocalRotation();
		Vector3D scale = m_owner->getLocalScale();

		reactphysics3d::Vector3 react_pos(position.m_x, position.m_y, position.m_z);
		reactphysics3d::Quaternion react_rot = reactphysics3d::Quaternion::fromEulerAngles(rotation.m_x, rotation.m_y, rotation.m_z);

		//Transform transform; transform.setFromOpenGL(m_owner->getPhysicsLocalMatrix());
		reactphysics3d::Transform transform = reactphysics3d::Transform(react_pos, react_rot);
		Capsule* capsule = dynamic_cast<Capsule*>(owner);
		float caps = (capsule->capsuleHeight + capsule->m_radius) * 2;
		CapsuleShape* capsuleShape = physicsCommon->createCapsuleShape(capsule->m_radius, caps);

		m_rigid_body = physicsWorld->createRigidBody(transform);
		m_rigid_body->addCollider(capsuleShape, reactphysics3d::Transform::identity());
		m_rigid_body->updateMassPropertiesFromColliders();
		m_rigid_body->setMass(m_mass);
		m_rigid_body->setType(BodyType::DYNAMIC);

		////whenever a new physics component is initialized. Register to physics system
		//ComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
		//PhysicsCommon* physicsCommon = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
		//PhysicsWorld* physicsWorld = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

		//// Create a rigid body in the world
		//Vector3D scale = m_owner->getLocalScale();
		//Transform transform; transform.setFromOpenGL(m_owner->getPhysicsLocalMatrix());
		//Capsule* capsule = dynamic_cast<Capsule*>(owner);
		//float caps = 2 * (capsule->capsuleHeight + capsule->m_radius);
		//CapsuleShape* boxShape = physicsCommon->createCapsuleShape(capsule->m_radius, caps); //half extent

		//m_rigid_body = physicsWorld->createRigidBody(transform);
		//m_rigid_body->addCollider(boxShape, transform);
		//m_rigid_body->updateMassPropertiesFromColliders();
		//m_rigid_body->setMass(m_mass);
		//m_rigid_body->setType(BodyType::DYNAMIC);
		//
		//float matrix[16];
		//transform = m_rigid_body->getTransform();
		//transform.getOpenGLMatrix(matrix);

		//m_owner->setLocalMatrix(matrix);
	}
	else if(owner->getObjectType() == AGameObject::PLANE || owner->getObjectType() == AGameObject::PHYSICSPLANE)
	{
		//whenever a new physics component is initialized. Register to physics system
		ComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
		PhysicsCommon* physicsCommon = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
		PhysicsWorld* physicsWorld = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

		// Create a rigid body in the world
		Vector3D position = m_owner->getLocalPosition();
		Vector3D rotation = m_owner->getLocalRotation();
		Vector3D scale = m_owner->getLocalScale();

		reactphysics3d::Vector3 react_pos(position.m_x, position.m_y, position.m_z);
		reactphysics3d::Quaternion react_rot = reactphysics3d::Quaternion::fromEulerAngles(rotation.m_x, rotation.m_y, rotation.m_z);

		//Transform transform; transform.setFromOpenGL(m_owner->getPhysicsLocalMatrix());
		reactphysics3d::Transform transform = reactphysics3d::Transform(react_pos, react_rot);
		BoxShape* boxShape = physicsCommon->createBoxShape(Vector3(scale.m_x / 2, scale.m_y / 2, scale.m_z / 2)); //half extent

		m_rigid_body = physicsWorld->createRigidBody(transform);
		m_rigid_body->addCollider(boxShape, reactphysics3d::Transform::identity());
		m_rigid_body->updateMassPropertiesFromColliders();
		m_rigid_body->setMass(m_mass);
		m_rigid_body->setType(BodyType::STATIC);

		////whenever a new physics component is initialized. Register to physics system
		//ComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
		//PhysicsCommon* physicsCommon = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
		//PhysicsWorld* physicsWorld = ComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

		//// Create a rigid body in the world
		//Vector3D scale = m_owner->getLocalScale();
		//Transform transform; transform.setFromOpenGL(m_owner->getPhysicsLocalMatrix());
		//BoxShape* boxShape = physicsCommon->createBoxShape(Vector3(scale.m_x / 2, scale.m_y / 2, scale.m_z / 2)); //half extent

		//m_rigid_body = physicsWorld->createRigidBody(transform);
		//m_rigid_body->addCollider(boxShape, transform);
		//m_rigid_body->updateMassPropertiesFromColliders();
		//m_rigid_body->setMass(m_mass);
		//m_rigid_body->setType(BodyType::STATIC);

		//float matrix[16];
		//transform = m_rigid_body->getTransform();
		//transform.getOpenGLMatrix(matrix);

		//m_owner->setLocalMatrix(matrix);
	}
}

PhysicsComponent::~PhysicsComponent()
{
	ComponentSystem::getInstance()->getPhysicsSystem()->unregisterComponent(this);
}

void PhysicsComponent::preperform()
{
	Vector3D position = m_owner->getLocalPosition();
	Vector3D rotation = m_owner->getLocalRotation();

	reactphysics3d::Vector3 react_pos(position.m_x, position.m_y, position.m_z);
	reactphysics3d::Quaternion react_rot = reactphysics3d::Quaternion::fromEulerAngles(rotation.m_x, rotation.m_y, rotation.m_z);

	m_rigid_body->setTransform(reactphysics3d::Transform(react_pos, react_rot));
}

void PhysicsComponent::perform(float delta_time)
{
	/*float matrix[16];
	const Transform transform = m_rigid_body->getTransform();

	transform.getOpenGLMatrix(matrix);

	m_owner->setLocalMatrix(matrix);*/

	const Transform transform = m_rigid_body->getTransform();
	reactphysics3d::Vector3 reactPos = transform.getPosition();
	reactphysics3d::Quaternion quaternion = transform.getOrientation();

	Vector3D rotation;

	float x_sqr = quaternion.x * quaternion.x;
	float y_sqr = quaternion.y * quaternion.y;
	float z_sqr = quaternion.z * quaternion.z;

	rotation.m_x = std::atan2(2.0f * (quaternion.x * quaternion.w + quaternion.y * quaternion.z), 1.0f - 2.0f * (x_sqr + y_sqr));

	float sinp = 2.0f * (quaternion.y * quaternion.w - quaternion.x * quaternion.z);
	if (std::abs(sinp) >= 1.0f) rotation.m_y = std::copysign(PI / 2.0f, sinp);
	else rotation.m_y = std::asin(sinp);

	rotation.m_z = std::atan2(2.0f * (quaternion.x * quaternion.y + quaternion.z * quaternion.w), 1.0f - 2.0f * (y_sqr + z_sqr));

	m_owner->setPosition(reactPos.x, reactPos.y, reactPos.z);
	m_owner->setRotation(rotation);
}

RigidBody* PhysicsComponent::getRigidBody()
{
    return m_rigid_body;
}
