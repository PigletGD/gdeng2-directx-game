#pragma once
#include "AComponent.h"

#include <reactphysics3d/reactphysics3d.h>

using namespace reactphysics3d;

class PhysicsComponent : public AComponent
{
public:
	PhysicsComponent(String name, AGameObject* owner);
	~PhysicsComponent();

	void preperform();
	void perform(float delta_time) override;

	RigidBody* getRigidBody();

	RigidBody* m_rigid_body;
private:
	float m_mass = 1000.0f; //in kilograms
	
};