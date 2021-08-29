#pragma once
#include "Plane.h"
class PhysicsPlane : public Plane
{
public:
	PhysicsPlane(String name, bool skipInit = false);
	~PhysicsPlane();
};