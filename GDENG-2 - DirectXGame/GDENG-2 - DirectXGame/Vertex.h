#pragma once

#include "Vector2D.h"
#include "Vector3D.h"

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};

struct vertex_color
{
	Vector3D position;
	Vector3D position1;
	Vector3D color;
	Vector3D color1;
};