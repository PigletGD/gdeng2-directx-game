#pragma once

#include "Vector3D.h"
#include "ConstantData.h"
#include "Prerequisites.h"

#include <string>

class AGameObject
{
public:
	AGameObject(std::string name);
	~AGameObject();

	virtual void update(float deltaTime);
	virtual void draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader, constant cc);

	void setPosition(float x, float y, float z);
	void setPosition(Vector3D pos);
	Vector3D getLocalPosition();

	void setRotation(float x, float y, float z);
	void setRotation(Vector3D pos);
	Vector3D getLocalRotation();

	void setScale(float x, float y, float z);
	void setScale(Vector3D pos);
	Vector3D getLocalScale();

private:
	std::string m_name;
	Vector3D m_local_position;
	Vector3D m_local_rotation;
	Vector3D m_local_scale;
};