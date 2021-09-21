#pragma once
#include "AGameObject.h"
#include "PhysicsComponent.h"


class Capsule :
    public AGameObject
{
public:
	Capsule(std::string name, AGameObject::PrimitiveType type);
	~Capsule();

	void update(float delta_time) override;
	void draw(int width, int height) override;

	void saveEditState() override;
	void restoreEditState() override;

	float m_radius = 0.5f;
	float capsuleHeight = 0.5f;//0.5f
protected:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	PhysicsComponent* m_component_attached;
	
	float m_ticks = 0.0f;
};

