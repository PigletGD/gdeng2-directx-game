#pragma once
#include "Plane.h"
class PhysicsPlane : public Plane
{
public:
	PhysicsPlane(String name, AGameObject::PrimitiveType type, bool skipInit = false);
	~PhysicsPlane();
};