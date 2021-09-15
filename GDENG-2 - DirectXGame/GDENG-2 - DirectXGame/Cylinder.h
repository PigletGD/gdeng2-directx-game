#pragma once
#include "AGameObject.h"

class Cylinder : public AGameObject
{
public:
	Cylinder(std::string name, AGameObject::PrimitiveType type);
	~Cylinder();

	void update(float delta_time) override;
	void draw(int width, int height) override;

protected:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	float m_ticks = 0.0f;
};

