#pragma once

#include "Vector2D.h"
#include "Vector3D.h"

struct vertex
{
	Vector3D position;
	Vector3D color;
};

struct lerp_vertex
{
	Vector3D position;
	Vector3D position1;
	Vector3D color;
	Vector3D color1;
};

struct textured_vertex
{
	Vector3D position;
	Vector2D texcoord;
};