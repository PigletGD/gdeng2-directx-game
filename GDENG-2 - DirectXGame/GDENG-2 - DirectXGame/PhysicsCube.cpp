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
	//component->getRigidBody()->setType(BodyType::KINEMATIC);
}

PhysicsCube::~PhysicsCube()
{
}

void PhysicsCube::update(float deltaTime)
{
	Cube::update(deltaTime);
}

void PhysicsCube::draw(int width, int height)
{
	Cube::draw(width, height);
}
