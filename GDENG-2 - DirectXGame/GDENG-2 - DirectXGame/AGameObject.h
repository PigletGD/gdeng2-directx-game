#pragma once

#include "Vector3D.h"
#include "Matrix4x4.h"
#include "ConstantData.h"
#include "Prerequisites.h"

#include <string>

class VertexShader;
class PixelShader;

class AGameObject
{
public:
	typedef std::string String;

	AGameObject(String name);
	~AGameObject();

	virtual void update(float deltaTime);
	virtual void draw(int width, int height);

	void setPosition(float x, float y, float z);
	void setPosition(Vector3D pos);
	Vector3D getLocalPosition();

	void setRotation(float x, float y, float z);
	void setRotation(Vector3D pos);
	Vector3D getLocalRotation();

	void setScale(float x, float y, float z);
	void setScale(Vector3D pos);
	Vector3D getLocalScale();

	bool isEnabled();
	void setEnabled(bool flag);

	String getName();
	void setName(String name);

protected:
	String m_name;
	Vector3D m_local_position;
	Vector3D m_local_rotation;
	Vector3D m_local_scale;
	Matrix4x4 m_local_matrix;

private:
	bool m_enabled = true;
};