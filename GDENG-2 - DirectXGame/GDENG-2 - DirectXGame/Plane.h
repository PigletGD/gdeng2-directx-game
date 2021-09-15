#pragma once
#include "AGameObject.h"

#include "ConstantData.h"

class Plane : public AGameObject
{
public:
	Plane(std::string name, AGameObject::PrimitiveType type, bool skip_init = false);
	~Plane();

	void update(float deltaTime) override;
	void draw(int width, int height) override;

private:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	float m_ticks = 0.0f;
};