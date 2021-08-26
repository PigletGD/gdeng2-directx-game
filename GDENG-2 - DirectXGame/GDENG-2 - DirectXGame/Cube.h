#pragma once
#include "AGameObject.h"

class Cube : public AGameObject
{
public:
	Cube(std::string name, bool skip_init = false);
	~Cube();

	void update(float delta_time) override;
	virtual void draw(int width, int height) override;

protected:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	float m_ticks = 0.0f;
};

