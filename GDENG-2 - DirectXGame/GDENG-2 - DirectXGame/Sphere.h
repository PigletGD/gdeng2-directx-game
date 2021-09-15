#pragma once
#include "AGameObject.h"

class Sphere : public AGameObject
{
public:
	Sphere(std::string name, AGameObject::PrimitiveType type);
	~Sphere();

	void update(float delta_time) override;
	void draw(int width, int height) override;

protected:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	float m_ticks = 0.0f;
};
