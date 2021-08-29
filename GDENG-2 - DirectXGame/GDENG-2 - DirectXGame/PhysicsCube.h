#pragma once
#include "Cube.h"
class PhysicsCube : public Cube
{
public:
	PhysicsCube(String name, bool skipInit = false);
	~PhysicsCube();

	void update(float deltaTime) override;
	void draw(int width, int height) override;

protected:
	float m_mass = 3.0f;
};