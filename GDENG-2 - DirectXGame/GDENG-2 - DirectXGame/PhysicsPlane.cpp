#include "PhysicsPlane.h"
#include "PhysicsComponent.h"
#include "GraphicsEngine.h"
#include "ShaderLibrary.h"
#include "ComponentSystem.h"
#include "PhysicsSystem.h"
#include "MathUtils.h"
#include <stdlib.h>

PhysicsPlane::PhysicsPlane(String name, AGameObject::PrimitiveType type, bool skipInit) : Plane(name, type, skipInit)
{
	setPosition(0.0f, -10.0f, 0.0f);
	setScale(32.0f, 0.05f, 32.0f);
	updateLocalMatrix();

	attachComponent(new PhysicsComponent("PhysicsComponent", this));

	PhysicsComponent* component = (PhysicsComponent*)this->findComponentOfType(AComponent::ComponentType::Physics, "PhysicsComponent");
	component->getRigidBody()->setType(BodyType::KINEMATIC);
}

PhysicsPlane::~PhysicsPlane()
{
}