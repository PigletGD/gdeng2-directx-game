#pragma once
#include "AGameObject.h"
#include "PhysicsComponent.h"

class Cube : public AGameObject
{
public:
	Cube(std::string name, AGameObject::PrimitiveType type, bool skip_init = false);
	~Cube();

	void update(float delta_time) override;
	void saveEditState() override;
	void restoreEditState() override;
	virtual void draw(int width, int height) override;

protected:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	PhysicsComponent* m_component_attached;
	
	float m_ticks = 0.0f;
};

