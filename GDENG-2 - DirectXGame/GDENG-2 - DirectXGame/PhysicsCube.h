#pragma once
#include "Cube.h"
#include "PhysicsComponent.h"

class PhysicsCube : public Cube
{
public:
	PhysicsCube(String name, AGameObject::PrimitiveType type, bool skipInit = false);
	~PhysicsCube();

	void update(float deltaTime) override;
	void draw(int width, int height) override;

	void saveEditState() override;
	void restoreEditState() override;

protected:
	float m_mass = 3.0f;

	//PhysicsComponent* m_component_attached;

	void awake() override;
};