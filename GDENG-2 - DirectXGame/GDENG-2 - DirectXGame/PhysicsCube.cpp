#include "PhysicsCube.h"
#include "PhysicsComponent.h"

PhysicsCube::PhysicsCube(String name, bool skipInit) : Cube(name, skipInit)
{
	setPosition(0.0f, 10.0f, 0.0f);
	setScale(1.0f, 1.0f, 1.0f);
	updateLocalMatrix();

	attachComponent(new PhysicsComponent("PhysicsComponent", this));

	PhysicsComponent* component = (PhysicsComponent*)findComponentOfType(AComponent::ComponentType::Physics, "PhysicsComponent");
	component->getRigidBody()->setMass(m_mass);
}

PhysicsCube::~PhysicsCube()
{
	detachComponent(m_component_attached);
	delete m_component_attached;
	Cube::~Cube();
}

void PhysicsCube::update(float deltaTime)
{
	Cube::update(deltaTime);
}

void PhysicsCube::draw(int width, int height)
{
	Cube::draw(width, height);
}

void PhysicsCube::awake()
{
	AGameObject::awake();
	m_component_attached = new PhysicsComponent("PhysicsComponent_" + m_name, this);
	attachComponent(m_component_attached);
}

void PhysicsCube::saveEditState()
{
	AGameObject::saveEditState();
}

void PhysicsCube::restoreEditState()
{
	AGameObject::restoreEditState();
	detachComponent(m_component_attached);
	delete m_component_attached;

	//also restore physics by redeclaring component
	m_component_attached = new PhysicsComponent("PhysicsComponent_" + m_name, this);
	attachComponent(m_component_attached);
}
